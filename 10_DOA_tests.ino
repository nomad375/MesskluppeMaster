/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests() {

 // Serial.println("Start of DOA");

  radio.setChannel(77); //77 in the past
  radio.openWritingPipe(pipes[1]);            // Where we send data out DOA pipes[1]. Default Pipes[0]!!!!!
  radio.setPALevel (RF24_PA_HIGH);
  //radio.setDataRate (RF24_250KBPS);
  //radio.setDataRate (RF24_2MBPS);



  /*======================================================================================
          Do your tests here:
    ======================================================================================== */


//  for (int TestLoop = 0; TestLoop < 65535; TestLoop++) {ReadSensors(g_DataSensors);} //just a test for readSensors function.





  //GetRadioConnection();
  //DeleteAllFiles();

  /* HERE we can create new file list */
//CreateFileList();
//SendFile("files/file.dir", 1,65535,40);

  /* HERE we can constanly send data online. time limit inside function SendOnline() */

//SendOnline();


  /* HERE we can сreate new file and immidiately send it. Time for logging in global variables or below */
    g_maxMeasurement = 1000 * 60 * 3;                      // ligging tume in milliseconds
    IamInOven = true;
    IamAtInlet = false;
    StartMesurment();
    g_task= 40;
    SendFile(g_FileName, 1, 65535, g_task);


  /* HERE we can send by  file name */
//  g_task = 40;
//  strncpy(g_FileName, "1538401938.csv", 15); // in case sending certain file - do STRNCPY, name, 12.
//  Serial.println (g_FileName);
//  SendFile(g_FileName, 1, 10000, g_task);

  //mode_ping();
  Serial.println(" end of DOA");
  delay(7000);
  //  /*======================================================================================
  //          Reboot Arduino Zero
  //    ======================================================================================== */
  //  delay(600000);
  NVIC_SystemReset(); //reset CPU function
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


