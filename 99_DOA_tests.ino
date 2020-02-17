/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests() {

  Serial.println("Start of DOA");

  radio.setChannel(77); //77 in the past
  radio.openWritingPipe(pipes[1]);            // Where we send data out DOA pipes[1]. Default Pipes[0]!!!!!
  radio.setPALevel (RF24_PA_HIGH);
  //radio.setDataRate (RF24_250KBPS);
  //radio.setDataRate (RF24_2MBPS);

  digitalWrite(5, HIGH);//wake up INA125
  IamInOven = false;
  IamAtInlet = false;
  
  delay(250);


      attachInterrupt(INTERRUPT_PIN_INLET, IRQ1, RISING); //atach interupt to wait we are at inlet...
      attachInterrupt(INTERRUPT_PIN_CLIP, IRQ2, CHANGE); //atach interupt to wait we are in oven...
  
  uint16_t Tclip, Tboard, Vbat;



while (1) { //test while loop
  delay(100);
//  SdCardErrorsCheck();
  
//
//  Serial.print(" IRQ1 = "); Serial.print(digitalRead(0));
//  Serial.print(" IRQ2 = ");Serial.print(digitalRead(1));
//  Serial.print(" IamAtInlet = ");Serial.print(IamAtInlet);
//  Serial.print(" IamInOven = ");Serial.print(IamInOven);   
//  Serial.println();
//  
ReadSensors(g_DataSensors);
Watchdog.reset();

 //   delay (1000);
  //   for (byte i = 0; i<10; i++)  {Serial.print (g_DataSensors[i]); Serial.print(", "); } Serial.println("  ");
  //   for (byte i = 0; i<10; i++)  {Serial.print ((int16_t)g_DataSensors[i]); Serial.print(", "); } Serial.println("  ");
    
//   Serial.print (" X force: "); 
//   Serial.print ((int16_t)g_DataSensors[1]);
//   Serial.print (" [N], Y force: ");   
//   Serial.print ((int16_t)g_DataSensors[2]);
//   Serial.print (" [N], Z force: ");  
//   Serial.print ((int16_t)g_DataSensors[3]);
//   Serial.println (" [N]");
//  Serial.print (" Temperature 7: "); Serial.println (g_DataSensors[7]);// g_DataSensors[7]
//  Serial.print (" Temperature 8: "); Serial.println (g_DataSensors[8]);
}//end test whilw loop




  /*======================================================================================
          Create files
    ======================================================================================== */

//  for (int TestLoop = 1; TestLoop < 10; TestLoop++) {
//      g_maxMeasurement = 1000 * TestLoop*3;
//  
//      IamInOven = true;
//      IamAtInlet = false;
//      g_task= 41;
//      Serial.println ("logging to DAT");
//      StartMesurment();
//  
//   } //just a test for readSensors function.



  CreateFileList();

  //GetRadioConnection();
  //DeleteAllFiles();

  /* HERE we can create new file list */
  //
  //    g_task= 41;
  //    g_maxMeasurement = 1000*3;
  ////   Serial.println ("logging to DAT");
  ////    IamInOven = true;
  ////    IamAtInlet = false;
  //    StartMesurment();
  //    g_task= 41;
  //    SendDatFile(g_FileName, 1, 65535, g_task);
  //CreateFileList();
  //g_task= 30;
  //SendTxtFile("files/file.dir", 1,65535,g_task);

  /* HERE we can constanly send data online. time limit inside function SendOnline() */

  SendOnline();


  /* HERE we can сreate new file and immidiately send it. Time for logging in global variables or below */
  //    g_maxMeasurement = 1000 * 60 * 3;                      // logging tume in milliseconds
  //    IamInOven = true;
  //    IamAtInlet = false;
  //    StartMesurment();
  //    g_task= 41;
  //    SendDatFile(g_FileName, 1, 65535, g_task);
  //    Serial.println ("+++++++++++++++++");
  //
  //
  //    g_maxMeasurement = 1000 * 60 * 3;                      // logging tume in milliseconds
  //    IamInOven = true;
  //    IamAtInlet = false;
  //    StartMesurmentCSV();
  //    g_task= 40;
  //    SendTxtFile(g_FileName, 1, 65535, g_task);
  //    Serial.println ("+++++++++++++++++");
  //    delay(3000);

  //
  //    g_maxMeasurement = 1000 * 60 * 5;                      // ligging tume in milliseconds
  //    IamInOven = true;
  //    IamAtInlet = false;
  //    StartMesurment();
  //    g_task= 41;
  //    SendFile(g_FileName, 1, 65535, g_task);



  /* HERE we can send by  file name */
//    g_task = 41;
//    strncpy(g_FileName, "1543417910.dat", 15); // in case sending certain file - do STRNCPY, name, 12.
//    Serial.println (g_FileName);
//    SendDatFile(g_FileName, 1, 65535, g_task);

  //    g_task = 40;
  //  strncpy(g_FileName, "1538740580.csv", 15); // in case sending certain file - do STRNCPY, name, 12.
  //  Serial.println (g_FileName);
  //  SendFile(g_FileName, 1, 19778, g_task);
  //


  //mode_ping();
  Serial.println(" end of DOA");
  delay(7000);
  //  /*======================================================================================
  //          Reboot Arduino Zero
  //    ======================================================================================== */
  delay(600000);
  // NVIC_SystemReset(); //reset CPU function
  //
  //
  //
  //  /*======================================================================================
  //          Change everething back :)
  //    ======================================================================================== */
  //
  radio.setChannel(77); //77 in the past
  radio.openWritingPipe(pipes[0]);            // Where we send data out DOA pipes[1]. Defoult Pipes[0]!!!!!
  radio.setPALevel (RF24_PA_HIGH);
  radio.setDataRate (RF24_1MBPS);


}//end of DOAtests()
