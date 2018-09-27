/*=========================================================================
                         Send File
    -----------------------------------------------------------------------*/

void SendFile(char *g_FileName, uint32_t FirstLine, uint32_t LinesToSend, uint16_t task){ //Start SendData() - based on fgets example from SdFat library
    Serial.print ("This file sending now: " ); Serial.println(g_FileName);
    
    /*========== Variables ==========*/
    uint32_t idTask = g_clipID*1000+task;
    char line[64];                          // char[48] enough for 7 sensors to read in buffer                       
    unsigned long startTime, stopTime = 0; 
    unsigned long timeoutPeriod = 3000;
    uint32_t SendMsg[8] = {idTask, 0, 0, 0, 0, 0, 0, 0};
    
    /*======== Radio Settings =======*/
    radio.powerUp();
    radio.setRetries(2,15);  

    /*=========== SD Card ===========*/
    SdCardErrorsCheck(); 
    digitalWrite(8, HIGH);
    SdFile rdfile(g_FileName, O_READ);  // Open the File
    
    if (!rdfile.isOpen()) {
        SdCardErrorsCheck(); 
        exit(0);                        //exit from function if SD file cannot be opened
    }// endif
    rdfile.seekSet(FirstLine*35);       //TAKE care that 35 is smaller than and average srting volume in bytes but close enough to it

      
    /*==========================================
     *  Try to get a connection
    /*========================================== */
    radio.writeFast(&SendMsg,sizeof(SendMsg));                // Send the transfer command
    
    if(radio.txStandBy(timeoutPeriod)){                       // If transfer initiation was successful, do the following
        startTime = millis();                                 // For calculating transfer rate
        boolean timedOut = 0;                                 // Boolean for keeping track of failures

        while (rdfile.fgets(line, sizeof(line)) > 0) {       // Read the File line by line
           ConvertStringToArray(line, g_SendMsg);      
           g_SendMsg[0] = idTask;
          
           if(!radio.writeBlocking(&g_SendMsg,sizeof(g_SendMsg),timeoutPeriod)){  // If retries are failing and the user defined timeout is exceeded
              timedOut = 1;                                                       // Indicate failure
              break;                                                              // Break out of the for loop
           }//endif  
        }//endwhile   
        
        stopTime = millis();                                                      // Capture the time of completion or failure
   
       //This should be called to wait for completion and put the radio in standby mode after transmission, returns 0 if data still in FIFO (timed out), 1 if success
       if(timedOut){ 
          radio.txStandBy(); }                                                    //Partially blocking standby, blocks until success or max retries. FIFO flushed if auto timeout reached
       else{ 
          radio.txStandBy(timeoutPeriod);
       }// endif timedout                                                         //Standby, block until FIFO empty (sent) or user specified timeout reached. FIFO flushed if user timeout reached.
   
   }else{                                             
      Serial.println("Communication not established");                            //If unsuccessful initiating transfer, exit and retry later
   }

    /*==========================================
     *  Print the result
    /*========================================== */

    float rate = LinesToSend * 32 / (stopTime - startTime);         
    float lps = LinesToSend / ((stopTime - startTime)/1000);

    cout << "Transfer completed at " << rate << " KB/s" << endl;
    cout << "The transfer took " << stopTime - startTime << " ms at " << lps << " lines/s" << endl;

    /*==========================================
     *  lets go back to ping
    /*========================================== */
    digitalWrite(8, LOW);
    radio.powerDown();
}
