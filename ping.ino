/*=========================================================================
                         mode_ping()
    -----------------------------------------------------------------------*/ 
   void mode_ping(uint16_t task){
      Serial.println("<---------------------------------------- New Mode_Ping ---------------------------------------->");
      /*======== Radio Settings =======*/
        radio.powerUp();
        radio.setRetries(1, 2);
        
      /*========== Variables ==========*/
      uint32_t idTask = g_clipID*1000+task;
      uint32_t SendMsg[8] = {idTask, millis(), 0, 0, 0, 0, 0, 0};
      uint32_t RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      uint32_t ping = 65000;
      int sleep = 10;   

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
        /*======= Prepare SendMsg =======*/
        SendMsg[1] = millis();                                // Actual running time
        SendMsg[2] = ping;                                    // calculated ping in ms
        SendMsg[3] = average * 100;                           // average is float so we have to remove the '.'
        SendMsg[4] = analogRead(A7)*2*g_AnalogToMV;       // Actual input voltage of Clip in mV. 4096 for 12-bits /1024 for 10-bits analog input
        ReadSensors(g_DataSensors);                           // Update Sensors
        SendMsg[7] = g_DataSensors[4]*0xFFFF+g_DataSensors[5]; // Temperatures [0]=Clip [1]=Board
        
        /*========= Send SendMsg ========*/
        //cout << "Send Ping: " << SendMsg[0] << "," <<  SendMsg[1] << "," << SendMsg[2] << "," << SendMsg[3] << "," << SendMsg[4] << "," << SendMsg[5] << "," << SendMsg[6] << "," << SendMsg[7] << endl;
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
        if (radio.isAckPayloadAvailable()) {
              radio.read(&RcvMsg, sizeof(RcvMsg));
              uint32_t task = RcvMsg[0]%1000;                 // calculate the task
              uint32_t id = (RcvMsg[0]-task)/1000;            // calculate the id
              ping = SendMsg[1] - RcvMsg[1];                  // calculate the ping
              //cout <<"Recv: " << RcvMsg[0] << "," << RcvMsg[1] << "," << RcvMsg[2] << "," << RcvMsg[3] << "," << RcvMsg[4] << "," << RcvMsg[5] << "," << RcvMsg[6] << "," << RcvMsg[7] << endl; 
              //cout << "task :" << task << " id:" << id << " ping: " << ping << endl;

              /*======= Check the msg =========*/
              if (id == g_clipID && ping < g_maxPing && ping > 0 && task != 0){
                  Serial.println(">> Got task >>");
                  
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
                  return;
              }//endIf "check the msg"
        }//endIf "Answer from RPI"

        /*======== Power Saving =========*/
        radio.powerDown();
        delay(sleep);

      } //endWhile "Loop until we get a task from Raspberry"
      /*==== reset Radio Settings =====*/
      radio.setRetries(2, 15); 
  }//endFunc "mode_ping"

