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
  bout << F(";Time");                                                                  // name the column for time in buffer
  bout << F(";Line#"); 
  bout << F(";ms%100 "); 
  bout << F(";FX "); 
  bout << F(";FY "); 
  bout << F(";FZ");
  bout << F(";AX "); 
  bout << F(";AY"); 
  bout << F(";AZ");
  bout << F(";GX"); 
  bout << F(";GY"); 
  bout << F(";GZ");
  bout << F(";Tbr"); 
  bout << F(";Tcl"); 
  bout << F(";MX"); 
  bout << F(";MY"); 
  bout << F(";MZ");
  bout << F(";Vin");
  
  logfile << buf << endl;                                                               //save buffer on sd
  
  while ((actTime - startTime) <= g_maxMeasurement) {  //collect datas as long as defined in glabal

    LineNumber ++;
    do {
      actTime = millis();
    }

    while (actTime % g_logInterval);    // wait for time to be a multiple of interval
    detachInterrupt(INTERRUPT_PIN_INLET); // Stop interrupt to prevent freezing
    detachInterrupt(INTERRUPT_PIN_CLIP); // Stop interrupt to prevent freezing


    obufstream bout(buf, sizeof(buf));                // use buffer to format line
                                
    DateTime now = rtc.now();
    ReadSensors(g_DataSensors);
    
    bout << g_clipID; 
    bout << ';' << now.unixtime();
    bout << ';' << LineNumber;
   
    for (uint16_t ia = 0; ia < 16; ia++) { // sensors values
      bout << ';' << (int16_t)g_DataSensors[ia];
    }   
      
    bout << endl;      // buffer the analog values

    logfile << buf;               //move data from buffer to file. DONT use flush HERE -> slow down saving data

   attachInterrupt(INTERRUPT_PIN_INLET, IRQ1, FALLING);
   attachInterrupt(INTERRUPT_PIN_CLIP, IRQ2, FALLING);

    if (!logfile) {
      //error("write logfile data failed");   //check for errors in logfile
      SdCardErrorsCheck();
  //    Serial.println ("No Logfile but SD card checked ");
      break;
    }

#if ECHO_TO_SERIAL                        //if EchoToSerial (line 36) =1 

    cout << buf;                          //the data will be printed on Serial Monitor
Serial.println();
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
