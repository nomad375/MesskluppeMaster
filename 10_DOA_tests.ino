/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests() {
 
 while (!Serial);

  radio.setChannel(77); //77 in the past
  radio.openWritingPipe(pipes[1]);            // Where we send data out DOA pipes[1]. Default Pipes[0]!!!!!
  radio.setPALevel (RF24_PA_HIGH);
//  radio.setDataRate (RF24_250KBPS);
  /*======================================================================================
          Do your tests here:
    ======================================================================================== */



  // ReadSensors(g_DataSensors); //just a test for Analog read function.
  //GetRadioConnection();
  //DeleteAllFiles();

/* HERE we can create new file list */ 
  //CreateFileList();
  //SendFile("files/file.dir", 1,65535);
  
/* HERE we can constanly send data online. time limit inside function SendOnline() */ 
  
  SendOnline();


 /* HERE we can сreate new file and immidiately send it. Time for logging in global variables or below */
//  g_maxMeasurement = 1000 * 60 * 0.1;                      // ligging tume in milliseconds
//  IamInOven = true;
//  IamAtInlet = false;
//  StartMesurment();
//  SendFile(g_FileName, 1, 65535, g_task);


/* HERE we can send by  file namew */
  // sprintf(g_FileName, "%10u.csv", "1530199472"); // name file as a seconds() since 01.01.1970.
  // strncpy(g_FileName, "1530199472.csv", 15); // in case sending certain file - do STRNCPY, name, 12.
  // Serial.println (g_FileName);
  // SendFile(g_FileName, 1, 4000, g_task);

  //mode_ping();
  Serial.println("end of DOA");
  /*======================================================================================
          Reboot Arduino Zero
    ======================================================================================== */
  delay(600000);
  NVIC_SystemReset(); //reset CPU function



  /*======================================================================================
          Change everething back :)
    ======================================================================================== */

  radio.setChannel(125); //77 in the past
  radio.openWritingPipe(pipes[0]);            // Where we send data out DOA pipes[1]. Defoult Pipes[0]!!!!!
  radio.setPALevel (RF24_PA_HIGH);
}//end of DOAtests()


