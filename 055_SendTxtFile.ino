/*=========================================================================
                         Send File
               ПРОВЕРИТЬ ОТПРАВКУ СПИСКА ФАЙЛОВ
    -----------------------------------------------------------------------*/

void SendTxtFile(char *g_FileName, uint16_t FirstLine, uint16_t LinesToSend, uint16_t task){ //Start SendData() - based on fgets example from SdFat library
    Serial.print ("This file sending now: " ); Serial.println(g_FileName);
    
    /*========== Variables ==========*/
    uint16_t idTask = g_clipID*1000+task;
    char line[128];                          // char[48] enough for 7 sensors to read in buffer                       
    unsigned long startTime, stopTime = 0; 
    unsigned long timeoutPeriod = 3000;
    uint16_t SendMsg[16] = {idTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


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
    
    rdfile.seekSet(FirstLine*70);       //TAKE care that 80 is smaller than and average srting volume in bytes but close enough to it


  
    /*========================================== 
     *  Try to get a connection
    /*========================================== */
    radio.writeFast(&SendMsg,sizeof(SendMsg));                // Send the transfer command
    
    if(radio.txStandBy(timeoutPeriod)){                       // If transfer initiation was successful, do the following
        startTime = millis();                                 // For calculating transfer rate
        boolean timedOut = 0;                                 // Boolean for keeping track of failures

       while (rdfile.fgets(line, sizeof(line)) > 0) 
       //for (int i = 0; i < 18000; i++)
        {       // Read the File line by line
       ConvertStringToArray(line, g_SendMsg, task);  

       g_SendMsg[0] = idTask;
       if (g_SendMsg[3] >= FirstLine){ // Send if line >= of desied fist line
    
            if(!radio.writeBlocking(&g_SendMsg,sizeof(g_SendMsg),timeoutPeriod)){  // If retries are failing and the user defined timeout is exceeded
                cout << "writeBlocking failed" << endl;       
                timedOut = 1;                                                       // Indicate failure
                break;                                                              // Break out of the for loop
             }//endif
             else {
              cout << "Line send: " << g_SendMsg[0] << "," << g_SendMsg[1] << "," << g_SendMsg[2] << "," << g_SendMsg[3] << "," << g_SendMsg[4] << "," << g_SendMsg[5] << "," << g_SendMsg[5] << endl;  
             }

      }//END if (g_SendMsg[3] >= FirstLine)
           if (g_SendMsg[3] >= FirstLine+LinesToSend){break;}
        }//endwhile   
        
        stopTime = millis();                                                      // Capture the time of completion or failure
   
       //This should be called to wait for completion and put the radio in standby mode after transmission, returns 0 if data still in FIFO (timed out), 1 if success
       if(timedOut){ 
          radio.txStandBy(); }                                                    //Partially blocking standby, blocks until success or max retries. FIFO flushed if auto timeout reached
       else{ 
          radio.txStandBy(timeoutPeriod);
       }// endif timedout                                                         //Standby, block until FIFO empty (sent) or user specified timeout reached. FIFO flushed if user timeout reached.
   
   }// end if radio
   
   else{                                             
      Serial.println("Communication not established");                            //If unsuccessful initiating transfer, exit and retry later
   } //end else

    /*==========================================
     *  Print the result
    /*========================================== */

    float rate = g_SendMsg[3] * 32 / (stopTime - startTime);         
    float lps = g_SendMsg[3] / ((stopTime - startTime)/1000);

    cout << "Transfer completed at " << rate << " KB/s" << endl;
    cout << "The transfer took " << stopTime - startTime << " ms at " << lps << " lines/s" << endl;

    /*==========================================
     *  lets go back to ping
    /*========================================== */
    digitalWrite(8, LOW);
    radio.powerDown();
} //end send file
