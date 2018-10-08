/*=========================================================================
                      StartMesurment DAT
  -----------------------------------------------------------------------*/

void StartMesurment() {


SdCardErrorsCheck(); 

  uint32_t startTime;          // variables for time in ms
  uint32_t actTime;
  actTime = startTime;
  uint32_t LineNumber = 0;
  uint32_t StartMicros, FinishMicros;


while ((millis()-startTime < g_timeout) && IamInOven == false) ; //wait clip interrupt or timeout
  if (IamInOven == false) { return;  } //go out of while, if not in oven

  startTime = millis();
  actTime = millis();
  digitalWrite(5, HIGH);//wake up INA125
  digitalWrite(8, HIGH);

  char FnameDAT[] = "0000000000.dat";                       //create a new file
  DateTime now = rtc.now();
  
   /* DAT file create */     
  sprintf(FnameDAT, "%10lu.dat", now.unixtime()); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
    if (sd.exists(FnameDAT)) {
    sprintf(FnameDAT, "%10lu.dat", now.unixtime()+1); // add one second if FnameCSV already exist. // by deafault %u changed to %lu by compilation warning
  }
  
datfile.open(FnameDAT, O_WRITE | O_CREAT);                                // open created file if creted new FnameCSV

  if (!datfile.isOpen()) {
    digitalWrite(8, HIGH);
    
    //error("file.open");       //if file not open -> error
    Serial.println("ERROR DAT file .open");       //if file not open -> error
    exit(0);
      }

Serial.print ("Logging to file " ); Serial.print (FnameDAT); Serial.print (" during " ); Serial.print (g_maxMeasurement/1000); Serial.println (" seconds " );
  
  while ((actTime - startTime) <= g_maxMeasurement) {  //collect datas as long as defined in glabal

    LineNumber ++;
    do {
      actTime = millis();
    }

    while (actTime % g_logInterval);    // wait for time to be a multiple of interval
    
    detachInterrupt(INTERRUPT_PIN_INLET); // Stop interrupt to prevent freezing
    detachInterrupt(INTERRUPT_PIN_CLIP); // Stop interrupt to prevent freezing

//StartMicros = micros();
ReadSensorsTMP(g_DataSensors);
//FinishMicros = micros(); Serial.print("ReadSensors() done micros "); Serial.println (FinishMicros - StartMicros); StartMicros=micros();  


/* DAT file write string */
//memset(&Payload, 0, sizeof(Payload));


Payload.Cell_0 = g_clipID; 
Payload.Cell_1_2 = now.unixtime(); //check is the same in CSV and DAT
Payload.Cell_3 = LineNumber;
Payload.Cell_4 = g_DataSensors[0];
Payload.Cell_5 = g_DataSensors[1];
Payload.Cell_6 = g_DataSensors[2];
Payload.Cell_7 = g_DataSensors[3];
Payload.Cell_8 = g_DataSensors[4];
Payload.Cell_9 = g_DataSensors[5];
Payload.Cell_10 = g_DataSensors[6];
Payload.Cell_11 = g_DataSensors[7];
Payload.Cell_12 = g_DataSensors[8];
Payload.Cell_13 = g_DataSensors[9];
Payload.Cell_14 = g_DataSensors[10];
Payload.Cell_15 = g_DataSensors[11];
Payload.Cell_16 = g_DataSensors[12];
Payload.Cell_17 = g_DataSensors[13];
Payload.Cell_18 = g_DataSensors[14];
Payload.Cell_19 = g_DataSensors[15];

PrintPayloadBytes();
//PrintPayloadHEX();

datfile.write((uint8_t *)&Payload, sizeof(Payload));

//FinishMicros = micros(); Serial.print("Line send to DAT file in micros "); Serial.println (FinishMicros - StartMicros);

//does not work for SdFile
//    if (!datfile) {
//      //error("write logfile data failed");   //check for errors in logfile
//      SdCardErrorsCheck();
//  //    Serial.println ("No Logfile but SD card checked ");
//      break;
//    }


   attachInterrupt(INTERRUPT_PIN_INLET, IRQ1, FALLING);
   attachInterrupt(INTERRUPT_PIN_CLIP, IRQ2, FALLING);


if (IamInOven == false) { break;  }

  } // enf of collect datas as long as defined in global g_maxMeasurement

  datfile.close();

  IamInOven = false;
  
  Serial.print("file saved in "); Serial.println ((millis() - startTime) / 1000); Serial.println(" seconds.");

 strncpy(g_FileName, FnameDAT, sizeof(FnameDAT) - 1); // Get FileName value for external use
//  Serial.print ("filename fo external use is:");Serial.println (g_FileName);
  //
  digitalWrite(5, LOW);  //sleep INA125
  digitalWrite(8, LOW);
  
} // END of StartMesurment DAT

/*=========================================================================
                      StartMesurment CSV
  -----------------------------------------------------------------------*/

void StartMesurmentCSV() {

SdCardErrorsCheck(); 

  uint32_t startTime;          // variables for time in ms
  uint32_t actTime;
  actTime = startTime;
  uint32_t LineNumber = 0;
  uint32_t StartMicros, FinishMicros;

    
while ((millis()-startTime < g_timeout) && IamInOven == false) ; //wait clip interrupt or timeout
  if (IamInOven == false) { return;  } //go out of while, if not in oven

  startTime = millis();
  actTime = millis();
  digitalWrite(5, HIGH);//wake up INA125
  digitalWrite(8, HIGH);

  char FnameCSV[] = "0000000000.csv";                       //create a new file
  DateTime now = rtc.now();
  
 /* CSV file create */ 
  sprintf(FnameCSV, "%10lu.csv", now.unixtime()); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
    if (sd.exists(FnameCSV)) {
  sprintf(FnameCSV, "%10lu.csv", now.unixtime()+1); // add one second if FnameCSV already exist. // by deafault %u changed to %lu by compilation warning
  }
  logfile.open(FnameCSV, O_WRITE | O_CREAT);                                // open created file if creted new FnameCSV
  //  logfile.open(FnameCSV);                                // open created file if creted new FnameCSV


  if (!logfile.is_open()) {
    digitalWrite(8, HIGH);
    
    //error("file.open");       //if file not open -> error
    Serial.println("ERROR CSV file .open");       //if file not open -> error
    exit(0);
      }

Serial.print ("Logging to file " ); Serial.print (FnameCSV); Serial.print (" during " ); Serial.print (g_maxMeasurement/1000); Serial.print (" seconds " );

 
  cout << F("Logging to: ") << FnameCSV << endl;            //Serialprint Logging to : FILENAME (+ End of Line)

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

//StartMicros = micros();
ReadSensorsTMP(g_DataSensors);
//FinishMicros = micros(); Serial.print("ReadSensors() done micros "); Serial.println (FinishMicros - StartMicros); StartMicros=micros();  

/* CSV file write string */
    obufstream bout(buf, sizeof(buf));                // use buffer to format line
    
    DateTime now = rtc.now();
    
    bout << g_clipID; 
    bout << ';' << now.unixtime();
    bout << ';' << LineNumber;
   
    for (uint16_t ia = 0; ia < 16; ia++) { // sensors values (all)
      bout << ';' << (int16_t)g_DataSensors[ia];
    }   
      
    bout << endl;      // buffer the analog values
    logfile << buf;               //move data from buffer to file. DONT use flush HERE -> slow down saving data

    if (!logfile) {
      //error("write logfile data failed");   //check for errors in logfile
      SdCardErrorsCheck();
  //    Serial.println ("No Logfile but SD card checked ");
      break;
    }

#if ECHO_TO_SERIAL                        //if EchoToSerial (line 36) =1 
    cout << buf;                          //the data will be printed on Serial Monitor
#endif  // ECHO_TO_SERIAL
//FinishMicros = micros(); Serial.print("Line send to CSV file in micros "); Serial.println (FinishMicros - StartMicros); StartMicros=micros();  

   attachInterrupt(INTERRUPT_PIN_INLET, IRQ1, FALLING);
   attachInterrupt(INTERRUPT_PIN_CLIP, IRQ2, FALLING);


if (IamInOven == false) { break;  }

  } // enf of collect datas as long as defined in global g_maxMeasurement

  logfile.close();

  IamInOven = false;
  Serial.print("file saved in "); Serial.println ((millis() - startTime) / 1000); Serial.println(" seconds.");

strncpy(g_FileName, FnameCSV, sizeof(FnameCSV) - 1); // Get FileName value for external use
//  Serial.print ("filename fo external use is:");Serial.println (g_FileName);
  //
  digitalWrite(5, LOW);  //sleep INA125
  digitalWrite(8, LOW);
  
} // ENF of StartMesurment
