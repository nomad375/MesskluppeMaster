/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests() {
  while (!Serial);

  radio.setChannel(125); //77 in the past
  radio.openWritingPipe(pipes[1]);            // Where we send data out DOA pipes[1]. Defoult Pipes[0]!!!!!

  /*======================================================================================
          Do your tests here:
    ======================================================================================== */
//CreateFileList();
//SendFile("files/file.dir", 1,65535);

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
//      for (int i = 1; i <= 99; i++) {
//        IamInOven = true;
//        g_timeout = 1000 * 1;
//        g_maxMeasurement = 1000 * int(random(10, 180));
//        StartMesurment();
//        delay(1000);
//      }
=======
=======
>>>>>>> parent of b6a41f1... lasf for now
=======
>>>>>>> parent of b6a41f1... lasf for now
      for (int i = 1; i <= 300; i++) {
        IamInOven = true;
        g_timeout = 1000 * 1;
        g_maxMeasurement = 1000 * int(random(7, 60));
        StartMesurment();
        delay(1000);
      }
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> parent of b6a41f1... lasf for now
=======
>>>>>>> parent of b6a41f1... lasf for now
=======
>>>>>>> parent of b6a41f1... lasf for now


  //CreateFileList();

 // SendOnline();

  // StartMesurment();
  //SendFile(g_FileName, 1,65535);

  //GetRadioConnection();

//DeleteAllFiles();
  Serial.println("end of DOA");
  /*======================================================================================
          Reboot Arduino Zero
    ======================================================================================== */

 // NVIC_SystemReset(); //reset CPU function

delay(60000);

  /*======================================================================================
          Change everething back :)
    ======================================================================================== */

//radio.setChannel(77); //77 in the past
//radio.openWritingPipe(pipes[0]);            // Where we send data out DOA pipes[1]. Defoult Pipes[0]!!!!!

}//end of DOAtests()
