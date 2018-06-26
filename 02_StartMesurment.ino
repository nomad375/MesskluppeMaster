/*=========================================================================
                      StartMesurment
  -----------------------------------------------------------------------*/

void StartMesurment() {

SdCardErrorsCheck(); 
  uint32_t startTime = millis();          // variables for time in ms
  uint32_t actTime;
  actTime = startTime;
  uint32_t LineNumber = 0;


  char Fname[] = "00000000.csv";                       //create a new file
  DateTime now = rtc.now();
  sprintf(Fname, "%08d.csv", now.unixtime() - 1514764800); // name file as a seconds() since 01.01.2018. valid untill 2021 year in case of 8.3 file name.
  if (sd.exists(Fname)) {
    sprintf(Fname, "%08d.csv", now.unixtime() - 1514764800 + 1); // add one second if Fname already exist
  }

  logfile.open(Fname);                                // open created file if creted new Fname
  if (!logfile.is_open()) {
    digitalWrite(8, HIGH);
    //error("file.open");       //if file not open -> error
    Serial.println("file.open");       //if file not open -> error
    exit;
      }

  cout << F("Logging to: ") << Fname << endl;            //Serialprint Logging to : FILENAME (+ End of Line)

  obufstream bout(buf, sizeof(buf));                                                    // format the buffer
  bout << F("Time ");                                                                  // name the column for time in buffer
  bout << F(";Microseconds "); 
  bout << F(";Line Number "); 
  for (uint8_t i = 1; i <= 4; i++) { // 4 analog inputs to read values
    bout << F(";S.") << int(i);  // name the columns of the sensors in buffer
  }
  logfile << buf << endl;                                                               //save buffer on sd
  
  while ((actTime - startTime) <= g_maxMeasurement) {  //collect datas as long as defined in line 13

    LineNumber ++;
    do {
      actTime = millis();
    }

    while (actTime % g_logInterval);    // wait for time to be a multiple of interval
    detachInterrupt(5); // Stop interrupt to prevent freezing
    detachInterrupt(9); // Stop interrupt to prevent freezing


    obufstream bout(buf, sizeof(buf));                // use buffer to format line
                                
    DateTime now = rtc.now();
    bout << g_clipID*1000+20; // 20(!?!)= SenfFile mode for RPI
    bout << ';' << now.unixtime();
    bout << ';' << millis()%1000; 
    bout << ';' << LineNumber;
    for (uint16_t ia = 1; ia <= 4; ia++) { // 4 analog inputs to read values
      bout << ';' << analogRead(ia);
    }   bout << endl;      // buffer the analog values

    logfile << buf;               //move data from buffer to file. DONT use flush HERE -> slow down saving data

   attachInterrupt(9, IRQ1, FALLING);
   attachInterrupt(5, IRQ2, FALLING);

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

  } // enf of collect datas as long as defined in line 13

  logfile.close();

  IamInOven = false;
  Serial.print("file saved in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds.");

  strncpy(g_FileName, Fname, sizeof(Fname) - 1); // Get FileName value for external use
  //

} // ENF of StartMesurment