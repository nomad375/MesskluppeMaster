/*=========================================================================
                         Send File
    -----------------------------------------------------------------------*/

void SendFile(char *g_FileName, uint32_t FirstLine, uint32_t LinesToSend, uint16_t task)

{ //Start SendData() - based on fgets example from SdFat library

 SdCardErrorsCheck(); 

  unsigned long counter = 0;
  uint32_t startTime = millis();
  char line[64];  // char[48] enough for 7 sensors to read in buffer
  int n; //counters
 // int i; //counters
 //int lost_package = 0;

  radio.powerUp();

  digitalWrite(8, HIGH);

  Serial.print ("This file sending now: " ); Serial.println(g_FileName);

  // open  file
  SdFile rdfile(g_FileName, O_READ);
  if (!rdfile.isOpen()) {
   // error("Fgets ERROR");
    SdCardErrorsCheck(); 
    exit(0); //exit from function if SD file cannot be opened
  }

  // read lines from the file
  
  rdfile.seekSet(FirstLine*35); //TAKE care that 35 is smaller than and average srting volume in bytes but close enough to it
  
  while ((n = rdfile.fgets(line, sizeof(line))) > 0) { // START read file line by line untill endOfFile
    
  ConvertStringToArray(line, g_SendMsg);
    g_SendMsg[0] = g_clipID*1000+task;



    Serial.print ("Line to send: "); for (byte iii = 0; iii<8; iii++){ Serial.print (g_SendMsg[iii]); Serial.print(", "); } Serial.println (" ");
    if (g_SendMsg[3] >= FirstLine){ // Send if line >= of desied fist line

          if (!radio.writeFast(&g_SendMsg, sizeof(g_SendMsg))) { counter++; Serial.println(counter); }
           //if (!radio.write(&g_SendMsg, sizeof(g_SendMsg))) { counter++; }
//           bool ok = radio.txStandBy(); 
//           if (ok == 0){lost_package = lost_package + 1; Serial.print("lost: "); Serial.println(lost_package);}
          if (counter > 1000){break;}
          


    }//END if

if (g_SendMsg[3] >= FirstLine+LinesToSend){break;}   // break if reach desied number of lines

if (IamAtInlet == true) { // Stop if come to Oven
 break;
} //end IF

  } // enf of read file line by line untill endOfFile

  Serial.print("SendFile done in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds."); 
  Serial.print(counter); Serial.println(" lines lost in space ");

  if (!radio.txStandBy()) {
    counter += 3;  // if radio.writeFast used its nesessary clean up FIFI buffer before TX mode
  }
  counter = 0;

  radio.powerDown();

//strncpy(g_FileName, "", 15); //delete filname from memory 13
  digitalWrite(8, LOW);
}// End of SendFile()

