/*=========================================================================
                         Send File
    -----------------------------------------------------------------------*/

void SendFile(char *g_FileName, uint32_t FirstLine, uint32_t LinesToSend)

{ //Start SendData() - based on fgets example from SdFat library

 SdCardErrorsCheck(); 

  unsigned long counter;
  uint32_t startTime = millis();
  char line[64];  // char[48] enough for 7 sensors to read in buffer
  int n, i; //counters

  radio.powerUp();

  digitalWrite(8, HIGH);

  Serial.print ("This file sending now: " ); Serial.println(g_FileName);

  // open  file
  SdFile rdfile(g_FileName, O_READ);
  if (!rdfile.isOpen()) {
   // error("Fgets ERROR");
   Serial.println("Fgets ERROR");
    SdCardErrorsCheck(); 
    exit; //exit from function if SD file cannot be opened
   // NVIC_SystemReset(); //reset CPU function
  }

  // read lines from the file
  
  rdfile.seekSet(FirstLine*35); //TAKE care that 38 is smaller than and average sting volume in bytes 
  
  while ((n = rdfile.fgets(line, sizeof(line))) > 0) { // START read file line by line untill endOfFile
  
    ConvertStringToArray(line, g_SendMsg);
  
    if (g_SendMsg[3] >= FirstLine){
           if (!radio.writeFast(&g_SendMsg, sizeof(g_SendMsg))) { counter++; }
    }//END if

if (g_SendMsg[3] >= FirstLine+LinesToSend){break;}   

if (IamAtInlet == true) { // Stop if come to Oven
 break;
}
  } // enf of read file line by line untill endOfFile

  Serial.print("SendFile done in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds.");

  if (!radio.txStandBy()) {
    counter += 3;  // if radio.writeFast used its nesessary clean up FIFI buffer before TX mode
  }
  counter = 0;

  radio.powerDown();

//strncpy(g_FileName, "", 13); //delete filname from memory 13
  digitalWrite(8, LOW);
}// End of SendFile()

