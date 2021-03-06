/*=========================================================================
                         mode_ping()
    -----------------------------------------------------------------------*/ 
   void mode_ping(uint16_t task){
    
      Serial.println("<---------------------------------------- New Mode_Ping ---------------------------------------->");
        digitalWrite(5, LOW);  Serial.println ("digitalWrite(5, LOW)");
        digitalWrite(13, HIGH);
      /*======== Radio Settings =======*/
        radio.powerUp();
        radio.setRetries(1, 2);

      /*========== Variables ==========*/
      uint16_t idTask = g_clipID*1000+task;
      uint16_t SendMsg[16] = {idTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      uint32_t RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      uint32_t ping = 999;
      int sleep = 10;   
      uint32_t myMillis;
      /*======== Average Array ========*/
      int numReadings = 25;             // How many 'package-statuses' are stored in the average array
      int statuses[numReadings];        // Is the array where we store the statuses
      int statusIndex = 0;              // the index of the current status
      float total = 0;                  // the running total
      float average = 0;                // the average
      bool sucess;                      // the package was transmited -> stored in statuses[]

      /*==== Prepare Average Array ====*/// Add 0 in all positions   
      for (int thisReading = 0; thisReading < numReadings; thisReading++) {
        statuses[thisReading] = 0;
      }

/*==========================================
 *  Lopp until we get a task from Raspberry
/*========================================== */
      while (true){
//       Serial.println("< i am waiting for task >"); 

 
        /*======= Prepare SendMsg =======*/
   myMillis = millis();      // Actual running time
        SendMsg[1] = (uint16_t) ((myMillis >> 16) & 0xFFFF) ;      //older bytes of uint32_t                        
        SendMsg[2] = (uint16_t) ((myMillis) & 0xFFFF) ;           //yanger bytes of uint32_t  
        SendMsg[3] = ping;                                    // calculated ping in ms
        SendMsg[4] = average * 100;                           // average is float so we have to remove the '.'
        SendMsg[5] = g_fileCount;
        
        /*========= Send SendMsg ========*/
        //cout << "Send Ping: " << SendMsg[0] << "," <<  myMillis << "," << SendMsg[3] << "," << SendMsg[4] << "," << SendMsg[5] << "," << SendMsg[6] << "," << SendMsg[7]<< "," << SendMsg[8] << "," << SendMsg[9] << "," << SendMsg[10] << "," << SendMsg[11] << "," << SendMsg[12] << ","<< SendMsg[13] << "," << SendMsg[14] << "," << SendMsg[15] << endl;
        radio.powerUp(); 
        sucess = radio.write(&SendMsg, sizeof(SendMsg));

        /*======== Average Calc =========*/
        total = total - statuses[statusIndex];                // subtract the last status: 
        statuses[statusIndex] = sucess;                       // add the actual status to the array
        total = total + statuses[statusIndex];                // add the actual status to the total
        statusIndex++;                                        // Next loop use the next array key
        if (statusIndex >= numReadings) {statusIndex = 0;}    // Reset index if maxmimum
        average = total / numReadings;                        // the average of the lost packages
          
        /*====== Answer from RPI  =======*/
        //memset(&RcvMsg, 0, sizeof(RcvMsg)); //clear RcvMsg
        if (radio.isAckPayloadAvailable()) {
              radio.read(&RcvMsg, sizeof(RcvMsg));
              uint32_t task = RcvMsg[0]%1000;                 // calculate the task
              uint32_t id = (RcvMsg[0]-task)/1000;            // calculate the id
              
              
              if ((myMillis - RcvMsg[1]) <= 65535){ping = (myMillis - RcvMsg[1]);}            // calculate the ping
              else {ping = 999;}                            // calculate the ping
  
              //cout <<"Recv: " << RcvMsg[0] << "," << RcvMsg[1] << "," << RcvMsg[2] << "," << RcvMsg[3] << "," << RcvMsg[4] << "," << RcvMsg[5] << "," << RcvMsg[6] << "," << RcvMsg[7] << endl; 
              //cout << "g_task :"<< g_task << ", task :" << task << ", id:" << id << ", ping: " << ping << endl;

              /*======= Check the msg =========*/
              //if (id == g_clipID && ping < g_maxPing && ping > 0 && task != 0){
              if (id == g_clipID && task != 0){
                
                  Serial.print(">> Got task >>"); Serial.println(task);
                  
                  // Store in globals
                  g_RcvMsg[0] = RcvMsg[0];                  // Clip idTask
                  g_RcvMsg[1] = RcvMsg[1];                  // Ping Timestamp
                  g_RcvMsg[2] = RcvMsg[2];                  // Real Time
                  g_RcvMsg[3] = RcvMsg[3];    
                  g_RcvMsg[4] = RcvMsg[4];
                  g_RcvMsg[5] = RcvMsg[5];
                  g_RcvMsg[6] = RcvMsg[6];
                  g_RcvMsg[7] = RcvMsg[7];
                  g_task = task;
                  break;

              }//endIf "check the msg"
        }//endIf "Answer from RPI"

        /*======== Power Saving =========*/
        radio.powerDown();
        delay(sleep);

    Watchdog.reset();

      } //endWhile "Loop until we get a task from Raspberry"

      digitalWrite(5, HIGH); Serial.println ("digitalWrite(5, HIGH)");
      digitalWrite(13, LOW);

      /*==== reset Radio Settings =====*/
      radio.setRetries(2, 15); 
      
return; 
  }//endFunc "mode_ping"
