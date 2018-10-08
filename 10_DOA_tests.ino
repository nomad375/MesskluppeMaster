/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests() {

 // Serial.println("Start of DOA");

  radio.setChannel(77); //77 in the past
  radio.openWritingPipe(pipes[1]);            // Where we send data out DOA pipes[1]. Default Pipes[0]!!!!!
  radio.setPALevel (RF24_PA_HIGH);
  //radio.setDataRate (RF24_250KBPS);
 // radio.setDataRate (RF24_2MBPS);



  /*======================================================================================
          Do your tests here:
    ======================================================================================== */

for (int TestLoop = 1; TestLoop < 100; TestLoop++) { 
    g_maxMeasurement = 1000 * TestLoop; 

        IamInOven = true;
    IamAtInlet = false;
    g_task= 41; 
    Serial.println ("logging to DAT");
    StartMesurment(); 

    IamInOven = true;
    IamAtInlet = false;
    g_task= 40; 
    Serial.println ("logging to CSV");
    StartMesurmentCSV();    

    
    
    } //just a test for readSensors function.





  //GetRadioConnection();
  //DeleteAllFiles();

  /* HERE we can create new file list */
    g_task= 41; 
    g_maxMeasurement = 1000*3; 
    Serial.println ("logging to DAT");
    IamInOven = true;
    IamAtInlet = false;
    StartMesurment();   
    g_task= 41;
    SendDatFile(g_FileName, 1, 65535, g_task);
CreateFileList();
g_task= 30;
SendTxtFile("files/file.dir", 1,65535,g_task);

  /* HERE we can constanly send data online. time limit inside function SendOnline() */

//SendOnline();


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
//  g_task = 41;
//  strncpy(g_FileName, "1538991205.dat", 15); // in case sending certain file - do STRNCPY, name, 12.
//  Serial.println (g_FileName);
//  SendDatFile(g_FileName, 1, 19778, g_task);
  
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


