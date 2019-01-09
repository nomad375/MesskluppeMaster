/*=========================================================================
                         Send File
    -----------------------------------------------------------------------*/

void SendDatFile(char *g_FileName, uint16_t FirstLine, uint16_t LinesToSend, uint16_t task){ //Start SendData() - based on fgets example from SdFat library
   Serial.print ("This file sending now:" ); Serial.println(g_FileName);
 
    
    /*========== Variables ==========*/
    uint16_t idTask = g_clipID*1000+task;
    unsigned long startTime, stopTime = 0; 
    unsigned long timeoutPeriod = 3000;
   
  memset(&Payload, 0, sizeof(Payload)); //memset(&structPointer,0,sizeof(structName)); //set a struct to all zeroes with memset
  Payload.Cell_0 = idTask;


    /*======== Radio Settings =======*/
    radio.powerUp();
    radio.setRetries(2,15);  

    /*=========== SD Card ===========*/
    SdCardErrorsCheck(); 
    digitalWrite(8, HIGH);

    SdFile datfile(g_FileName, O_READ);  // Open the File
    
    if (!datfile.isOpen()) {
        SdCardErrorsCheck(); 
        Serial.println ("cannot open dat file");
        exit(0);                        //exit from function if SD file cannot be opened
    }// endif
  //      Serial.println("datfile opened");
      
    /*==========================================
     *  Try to get a connection
    /*========================================== */
    radio.writeFast(&Payload.Cell_0,sizeof(Payload)-4);                // Send the transfer command
     PrintPayload();
    
    if(radio.txStandBy(timeoutPeriod)){                       // If transfer initiation was successful, do the following
        startTime = millis();                                 // For calculating transfer rate
        boolean timedOut = 0;                                 // Boolean for keeping track of failures


           while (datfile.available()) { //read until end of file
        
            datfile.read((uint8_t *)&Payload, sizeof(Payload)); // get from file blocks into Payload ( sizeof(Payload) )
            Payload.Cell_0 = idTask;
            
       PrintPayload();
        //PrintPayloadHEX();
        //PrintPayloadBytes();
        Serial.println();
        
        
                 if (Payload.Cell_3 >= FirstLine){ // Send if line >= of desied fist line
        
                          if(!radio.writeBlocking(&Payload.Cell_0,sizeof(Payload)-4,timeoutPeriod)){  // If retries are failing and the user defined timeout is exceeded
                             timedOut = 1;                                                            // Indicate failure
                             exit(0);                                                                 // Break out of the for loop
                          }//endif  
                          
                  }//END if (Payload.Cell_3 >= FirstLine)
             
                if (Payload.Cell_3 >= FirstLine+LinesToSend){break;}
           }// while while (datfile.available())    

       //This should be called to wait for completion and put the radio in standby mode after transmission, returns 0 if data still in FIFO (timed out), 1 if success
       if(timedOut){ 
          radio.txStandBy(); }                                                    //Partially blocking standby, blocks until success or max retries. FIFO flushed if auto timeout reached
       else{ 
          radio.txStandBy(timeoutPeriod);
       }// endif timedout                                                         //Standby, block until FIFO empty (sent) or user specified timeout reached. FIFO flushed if user timeout reached.
 
  stopTime = millis();                                                      // Capture the time of completion or failure        
    
  }// end if radio
   
   else{                                             
      Serial.println("Communication not established");                            //If unsuccessful initiating transfer, exit and retry later
   } //end else

    /*==========================================
     *  Print the result
    /*========================================== */

    float rate = Payload.Cell_3 * 32 / (stopTime - startTime);         
    float lps = Payload.Cell_3 / ((stopTime - startTime)/1000);

    cout << "Transfer completed at " << rate << " KB/s" << endl;
    cout << "The transfer took " << stopTime - startTime << " ms at " << lps << " lines/s" << endl;

    /*==========================================
     *  lets go back to ping
    /*========================================== */
    digitalWrite(8, LOW);
    radio.powerDown();
} //end send file


void PrintPayload()
{
Serial.print ("Payload (");
Serial.print (sizeof(Payload));
Serial.print (")bytes   DEC:  ");
Serial.print (Payload.Cell_ZERO); Serial.print (" "); 
Serial.print (Payload.Cell_0); Serial.print (" "); 
Serial.print (Payload.Cell_1_2); Serial.print (" "); 
Serial.print (Payload.Cell_3); Serial.print (" "); 
Serial.print (Payload.Cell_4); Serial.print (" "); 
Serial.print (Payload.Cell_5); Serial.print (" "); 
Serial.print (Payload.Cell_6); Serial.print (" "); 
Serial.print (Payload.Cell_7); Serial.print (" "); 
Serial.print (Payload.Cell_8); Serial.print (" "); 
Serial.print (Payload.Cell_9); Serial.print (" "); 
Serial.print (Payload.Cell_10); Serial.print (" "); 
Serial.print (Payload.Cell_11); Serial.print (" "); 
Serial.print (Payload.Cell_12); Serial.print (" "); 
Serial.print (Payload.Cell_13); Serial.print (" "); 
Serial.print (Payload.Cell_14); Serial.print (" "); 
Serial.print (Payload.Cell_15); Serial.print (" "); 


Serial.println ("");
}

void PrintPayloadHEX()
{

Serial.print ("Payload (");
Serial.print (sizeof(Payload));
Serial.print (")bytes   HEX:  ");
Serial.print (Payload.Cell_ZERO,HEX); Serial.print ("(");Serial.print (sizeof(Payload.Cell_ZERO));  Serial.print (") ");
Serial.print (Payload.Cell_0,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_0));  Serial.print (") ");
Serial.print (Payload.Cell_1_2,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_1_2));  Serial.print (") ");
Serial.print (Payload.Cell_3,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_3));  Serial.print (") ");
Serial.print (Payload.Cell_4,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_4));  Serial.print (") ");
Serial.print (Payload.Cell_5,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_5));  Serial.print (") ");
Serial.print (Payload.Cell_6,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_6));  Serial.print (") ");
Serial.print (Payload.Cell_7,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_7));  Serial.print (") ");
Serial.print (Payload.Cell_8,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_8));  Serial.print (") ");
Serial.print (Payload.Cell_9,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_9));  Serial.print (") ");
Serial.print (Payload.Cell_10,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_10));  Serial.print (") ");
Serial.print (Payload.Cell_11,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_11));  Serial.print (") ");
Serial.print (Payload.Cell_12,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_12));  Serial.print (") ");
Serial.print (Payload.Cell_13,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_13));  Serial.print (") ");
Serial.print (Payload.Cell_14,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_14));  Serial.print (") ");
Serial.print (Payload.Cell_15,HEX); Serial.print ("("); Serial.print (sizeof(Payload.Cell_15));  Serial.print (") ");

Serial.println ("");
}

void PrintPayloadBytes()
{

const byte * PayloadBytes = (const byte*) &Payload;  
uint8_t i;

Serial.print ("Payload (");
Serial.print (sizeof(Payload));
Serial.print (")bytes  PURE:");
for (i = 0; i < sizeof Payload; i++){ Serial.print(" ");Serial.print (PayloadBytes[i],HEX); }
Serial.println();

 // Serial.print("  Payload bytes:  "); for (byte i = 0; sizeof(Payload); i++)  {Serial.print (PayloadBytes[i],HEX); Serial.print(", "); } Serial.println(" done ");


}
