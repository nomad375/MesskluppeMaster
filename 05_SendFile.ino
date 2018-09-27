/*=========================================================================
                         Send File
    -----------------------------------------------------------------------*/

void SendFile(char *g_FileName, uint32_t FirstLine, uint32_t LinesToSend, uint16_t task)

{ //Start SendData() - based on fgets example from SdFat library

 SdCardErrorsCheck(); 

  unsigned long counter = 0;
 // uint32_t startTime = millis();
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



//=============================================================================================================
unsigned long rxTimer,startTime, stopTime, payloads = 0;  
bool TX=1,RX=0,role=0, transferInProgress = 0;
unsigned long timeoutPeriod = 3000;
radio.setRetries(15,15);  
unsigned long cycles = LinesToSend;   
unsigned long transferCMD[] = {'H','S',cycles }; 

 radio.writeFast(&transferCMD,12);                         // Send the transfer command
    if(radio.txStandBy(timeoutPeriod)){                       // If transfer initiation was successful, do the following
   
        startTime = millis();                                 // For calculating transfer rate
        boolean timedOut = 0;                                 // Boolean for keeping track of failures

        while ((n = rdfile.fgets(line, sizeof(line))) > 0) {
        //for(int i=0; i<cycles; i++){                          // Loop through a number of cycles
          ConvertStringToArray(line, g_SendMsg);
          g_SendMsg[0] = g_clipID*1000+task;
          
                                         // Change the first byte of the payload for identification
       
          if(!radio.writeBlocking(&g_SendMsg,sizeof(g_SendMsg),timeoutPeriod)){   // If retries are failing and the user defined timeout is exceeded
              timedOut = 1;                                   // Indicate failure
              counter = cycles;                               // Set the fail count to maximum
              break;                                          // Break out of the for loop
          }  
        }    
  
   
        stopTime = millis();                                  // Capture the time of completion or failure
   
       //This should be called to wait for completion and put the radio in standby mode after transmission, returns 0 if data still in FIFO (timed out), 1 if success
       if(timedOut){ radio.txStandBy(); }                     //Partially blocking standby, blocks until success or max retries. FIFO flushed if auto timeout reached
       else{ radio.txStandBy(timeoutPeriod);     }            //Standby, block until FIFO empty (sent) or user specified timeout reached. FIFO flushed if user timeout reached.
   
   }else{                                             
       Serial.println("Communication not established");       //If unsuccessful initiating transfer, exit and retry later
   }

    float rate = cycles * 32 / (stopTime - startTime);         //Display results:
    float lps = cycles / ((stopTime - startTime)/1000);
    
   Serial.print("Transfer complete at "); Serial.print(rate); printf(" KB/s \n\r");
   Serial.print(counter);
   Serial.print(" of ");
   Serial.print(cycles); Serial.println(" Packets Failed to Send");
   Serial.print("transfer took "); Serial.print(stopTime - startTime); Serial.print("ms = lines/s ");Serial.println(lps);
   counter = 0;   
//=============================================================================================================

//
//
//
//
//
//  
//  while ((n = rdfile.fgets(line, sizeof(line))) > 0) { // START read file line by line untill endOfFile
//    
//    ConvertStringToArray(line, g_SendMsg);
//    g_SendMsg[0] = g_clipID*1000+task;
//
//
//    
//
//
//
//    Serial.print ("Line to send: "); for (byte iii = 0; iii<8; iii++){ Serial.print (g_SendMsg[iii]); Serial.print(", "); } Serial.println (" ");
//    if (g_SendMsg[3] >= FirstLine){ // Send if line >= of desied fist line
//
//          if (!radio.writeFast(&g_SendMsg, sizeof(g_SendMsg))) { counter++; Serial.println(counter); }
//           //if (!radio.write(&g_SendMsg, sizeof(g_SendMsg))) { counter++; }
////           bool ok = radio.txStandBy(); 
////           if (ok == 0){lost_package = lost_package + 1; Serial.print("lost: "); Serial.println(lost_package);}
//          if (counter > 1000){break;}
//          
//
//
//    }//END if
//
//if (g_SendMsg[3] >= FirstLine+LinesToSend){break;}   // break if reach desied number of lines
//
//if (IamAtInlet == true) { // Stop if come to Oven
// break;
//} //end IF
//
//  } // enf of read file line by line untill endOfFile
//
//  Serial.print("SendFile done in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds."); 
//  Serial.print(counter); Serial.println(" lines lost in space ");
//
//  if (!radio.txStandBy()) {
//    counter += 3;  // if radio.writeFast used its nesessary clean up FIFI buffer before TX mode
//  }
//  counter = 0;

  radio.powerDown();

//strncpy(g_FileName, "", 15); //delete filname from memory 13
  digitalWrite(8, LOW);
}// End of SendFile()

