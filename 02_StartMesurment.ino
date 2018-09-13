/*=========================================================================
                      StartMesurment
  -----------------------------------------------------------------------*/

void StartMesurment() {


SdCardErrorsCheck(); 
  uint32_t startTime = millis();          // variables for time in ms
  uint32_t actTime;
  actTime = startTime;
  uint32_t LineNumber = 0;

while (millis()-startTime < g_timeout && IamInOven == false) ;
  if (IamInOven == false) { return;  }

  startTime = millis();
  actTime = millis();
 digitalWrite(5, HIGH);//wake up INA125
  digitalWrite(8, HIGH);

  char Fname[] = "0000000000.csv";                       //create a new file
  DateTime now = rtc.now();
  sprintf(Fname, "%10lu.csv", now.unixtime()); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
  if (sd.exists(Fname)) {
    sprintf(Fname, "%10lu.csv", now.unixtime()+1); // add one second if Fname already exist. // by deafault %u changed to %lu by compilation warning
  }
//Serial.println("file created: ");
  logfile.open(Fname);                                // open created file if creted new Fname
  if (!logfile.is_open()) {
    digitalWrite(8, HIGH);
    
    //error("file.open");       //if file not open -> error
    Serial.println("ERROR file.open");       //if file not open -> error
    exit(0);
      }

  cout << F("Logging to: ") << Fname << endl;            //Serialprint Logging to : FILENAME (+ End of Line)

  obufstream bout(buf, sizeof(buf));                                                    // format the buffer
  bout << F("ID");
  bout << F(";Time ");                                                                  // name the column for time in buffer
  bout << F(";Microseconds "); 
  bout << F(";Line Number "); 
  for (uint8_t i = 1; i <= 4; i++) { // 4 analog inputs to read values
    bout << F(";S.") << int(i);  // name the columns of the sensors in buffer
  }
  logfile << buf << endl;                                                               //save buffer on sd
  
  while ((actTime - startTime) <= g_maxMeasurement) {  //collect datas as long as defined in glabal

    LineNumber ++;
    do {
      actTime = millis();
    }

    while (actTime % g_logInterval);    // wait for time to be a multiple of interval
    detachInterrupt(20); // Stop interrupt to prevent freezing
    detachInterrupt(21); // Stop interrupt to prevent freezing


    obufstream bout(buf, sizeof(buf));                // use buffer to format line
                                
    DateTime now = rtc.now();
    bout << g_clipID; 
    bout << ';' << now.unixtime();
    bout << ';' << millis()%1000; 
    bout << ';' << LineNumber;
    
    ReadSensors(g_DataSensors);
    for (uint16_t ia = 1; ia <= 3; ia++) { // 4 analog inputs to read values
      bout << ';' << g_DataSensors[ia];
    }   
    bout << ';' << uint32_t( analogRead(A7)*2*ARef/4096*1000); //Actual input voltage of Clip in mV. 4096 for 12-bits /1024 for 10-bits analog input
   
    bout << endl;      // buffer the analog values

    logfile << buf;               //move data from buffer to file. DONT use flush HERE -> slow down saving data

   attachInterrupt(20, IRQ1, FALLING);
   attachInterrupt(21, IRQ2, FALLING);

    if (!logfile) {
      //error("write logfile data failed");   //check for errors in logfile
      SdCardErrorsCheck();
  //    Serial.println ("No Logfile but SD card checked ");
      break;
    }

#if ECHO_TO_SERIAL                        //if EchoToSerial (line 36) =1 
    cout << buf;                          //the data will be printed on Serial Monitor
#endif  // ECHO_TO_SERIAL

if (IamInOven == false) { break;  }

  } // enf of collect datas as long as defined in global g_maxMeasurement

  logfile.close();

  IamInOven = false;
  Serial.print("file saved in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds.");

  strncpy(g_FileName, Fname, sizeof(Fname) - 1); // Get FileName value for external use
  //
  digitalWrite(5, LOW);  //sleep INA125
  digitalWrite(8, LOW);
  
} // ENF of StartMesurment
