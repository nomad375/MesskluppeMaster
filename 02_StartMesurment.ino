/*=========================================================================
                      StartMesurment DAT
  -----------------------------------------------------------------------*/

void StartMesurment() {


  SdCardErrorsCheck();

  uint32_t startTime = millis();          // variables for time in ms
  uint32_t actTime;
  actTime = startTime;
  uint32_t LineNumber = 0;


  while ((millis() - startTime < g_timeout) && IamInOven == false) ; //wait clip interrupt or timeout
  if (IamInOven == false) {
    return;   //go out of while, if not in oven
  }

  startTime = millis();
  actTime = millis();

  char FnameDAT[] = "0000000000.dat";                       //create a new file
  DateTime now = rtc.now();

  /* DAT file create */
  sprintf(FnameDAT, "%10lu.dat", now.unixtime()); // name file as a seconds() since 01.01.1970. // by deafault %u changed to %lu by compilation warning
  if (sd.exists(FnameDAT)) {
    sprintf(FnameDAT, "%10lu.dat", now.unixtime() + 1); // add one second if FnameCSV already exist. // by deafault %u changed to %lu by compilation warning
  }

  datfile.open(FnameDAT, O_WRITE | O_CREAT);                                // open created file if creted new FnameCSV

  if (!datfile.isOpen()) {
    digitalWrite(8, HIGH);

    //error("file.open");       //if file not open -> error
    //Serial.println("ERROR DAT file .open");       //if file not open -> error
    exit(0);
  }

  Serial.print ("Logging to file " ); Serial.print (FnameDAT); Serial.print (" during " ); Serial.print (g_maxMeasurement / 1000); Serial.println (" seconds " );

  while ((actTime - startTime) <= g_maxMeasurement) {  //collect datas as long as defined in glabal

    LineNumber ++;
    do {
      actTime = millis();
    }

    while (actTime % g_logInterval);    // wait for time to be a multiple of interval


    ReadSensors(g_DataSensors);


    /* DAT file write string */
    //memset(&Payload, 0, sizeof(Payload));

    DateTime now = rtc.now();
    Payload.Cell_0 = g_clipID;
    Payload.Cell_1_2 = now.unixtime(); //check is the same in CSV and DAT
    Payload.Cell_3 = LineNumber;
    Payload.Cell_4 = g_DataSensors[0];    // readsensor time stamp
    Payload.Cell_5 = g_DataSensors[1];    // INA Load Cell 1
    Payload.Cell_6 = g_DataSensors[2];    // INA Load Cell 2
    Payload.Cell_7 = g_DataSensors[3];    // INA Load Cell 3
    Payload.Cell_8 = g_DataSensors[4];    // IMU Accel X
    Payload.Cell_9 = g_DataSensors[5];    // IMU Accel Y
    Payload.Cell_10 = g_DataSensors[6];   // Yaw angle
    Payload.Cell_11 = g_DataSensors[7];   // IMU Temperature
    Payload.Cell_12 = g_DataSensors[8];   // Analog Temperature Clip
    Payload.Cell_13 = g_DataSensors[9];   //  Analog V-Battery / 2
    Payload.Cell_14 = 00;   // Reserved Place
    Payload.Cell_15 = 00;   // Reserved Place


    //PrintPayload();
    //PrintPayloadBytes();
    //PrintPayloadHEX();

    detachInterrupt(INTERRUPT_PIN_INLET); // Stop interrupt to prevent freezing
    detachInterrupt(INTERRUPT_PIN_CLIP); // Stop interrupt to prevent freezing

    datfile.write((uint8_t *)&Payload, sizeof(Payload));


    attachInterrupt(INTERRUPT_PIN_INLET, IRQ1, FALLING); //atach interapt to see end of oven
    attachInterrupt(INTERRUPT_PIN_CLIP, IRQ2, FALLING); //atach interapt to see end of oven


    if (IamInOven == false) {
      break;
    }

  } // enf of collect datas as long as defined in global g_maxMeasurement

  datfile.close();

  IamInOven = false; //?

  Serial.print("file saved in "); Serial.println ((millis() - startTime) / 1000); Serial.println(" seconds.");

  strncpy(g_FileName, FnameDAT, sizeof(FnameDAT) - 1); // Get FileName value for external use



} // END of StartMesurment DAT
