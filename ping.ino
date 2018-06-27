/*=========================================================================
                         mode_ping()
    -----------------------------------------------------------------------*/ 
   void mode_ping(){
      Serial.println("<---------------------------------------- New Mode_Ping ---------------------------------------->");
      radio.powerUp();
      uint32_t idTask = g_clipID*1000;
      uint32_t SendMsg[8] = {idTask, micros(), 0, 0, 0, 0, 0, 0};
      uint32_t RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
      uint32_t timestamp = micros();
      
      while ((micros()-timestamp) < (g_maxPing)){

          SendMsg[1] = micros();
          radio.write(&SendMsg, sizeof(SendMsg));
          
          if ( radio.isAckPayloadAvailable() ) {
              radio.read(&RcvMsg, sizeof(RcvMsg));

               // Calculate the Ping
               uint32_t ping = SendMsg[1] - RcvMsg[1];
              cout << "ping :" << ping << endl;
               // Calculate the Task
               uint32_t task = RcvMsg[0]%1000;
               cout << "task :" << task << endl;
               //Calculate the ID
               uint32_t id = (RcvMsg[0]-task)/1000;
               cout << "id :" << id << endl;

               if (id == g_clipID && ping < g_maxPing && ping > 0 && task != 0){
                  Serial.println("i do");
                  
                  g_RcvMsg[0] = RcvMsg[0];    // Clip idTask
                  g_RcvMsg[1] = RcvMsg[1];    // Ping Timestamp
                  g_RcvMsg[2] = RcvMsg[2];    // Real Time
                  g_RcvMsg[3] = RcvMsg[3];    
                  g_RcvMsg[4] = RcvMsg[4];
                  g_RcvMsg[5] = RcvMsg[5];
                  g_RcvMsg[6] = RcvMsg[6];
                  g_RcvMsg[7] = RcvMsg[7];
                  g_task = task;
                  return;
               }


         }


      }
   }

