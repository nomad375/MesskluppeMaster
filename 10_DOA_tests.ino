/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests() {
//  while (!Serial);

  radio.setChannel(77); //77 in the past
  radio.openWritingPipe(pipes[1]);            // Where we send data out DOA pipes[1]. Default Pipes[0]!!!!!
  radio.setPALevel (RF24_PA_HIGH); 
  /*======================================================================================
          Do your tests here:
    ======================================================================================== */

//SendFile("files/file.dir", 1,65535);


//      for (int i = 1; i <= 3; i++) {
//        IamInOven = true;
//        g_timeout = 1000 * 1;
//        g_maxMeasurement = 1000 * int(random(180, 600));
//        StartMesurment();
//        delay(1000);
//      }

//CreateFileList();

  //CreateFileList();

 analogWrite(A0, 0); //output for A0 in 0-1023 bits (0 to 3,3v)
 //SendOnline();
 //mode_ping();

  // StartMesurment();
  //SendFile(g_FileName, 1,65535);

  //GetRadioConnection();

//DeleteAllFiles();

           // sprintf(g_FileName, "%10u.csv", "1530199472"); // name file as a seconds() since 01.01.1970. 
         //   strncpy(g_FileName, "1530199472.csv", 15); // in case sending certain file - do STRNCPY, name, 12.
          //  Serial.println (g_FileName);
         //   SendFile(g_FileName, 1, 4000, g_task);

  Serial.println("end of DOA");
  /*======================================================================================
          Reboot Arduino Zero
    ======================================================================================== */
delay(60000);
//NVIC_SystemReset(); //reset CPU function



  /*======================================================================================
          Change everething back :)
    ======================================================================================== */

radio.setChannel(125); //77 in the past
radio.openWritingPipe(pipes[0]);            // Where we send data out DOA pipes[1]. Defoult Pipes[0]!!!!!
radio.setPALevel (RF24_PA_HIGH); 
}//end of DOAtests()


