/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests(){
// while(!Serial);
//CreateFileList();
 radio.setChannel(111); //77 in the past
 radio.openWritingPipe(pipes[1]);            // Where we send data out DOA pipes[1]. Defoult Pipes[0]!!!!!
 SendOnline();

// StartMesurment();
//SendFile(g_FileName, 1,65535);

//GetRadioConnection();


Serial.println("end of DOA");
/*======================================================================================
 *      Reboot Arduino Zero
======================================================================================== */

//NVIC_SystemReset(); //reset CPU function


/*======================================================================================
 *      Change everething back :)
======================================================================================== */

radio.setChannel(77); //77 in the past
radio.openWritingPipe(pipes[0]);            // Where we send data out DOA pipes[1]. Defoult Pipes[0]!!!!!

}//end of DOAtests()
