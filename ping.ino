/*=========================================================================
                         mode_ping()
    -----------------------------------------------------------------------*/ 
   void mode_ping(uint16_t task){
      Serial.println("<---------------------------------------- New Mode_Ping ---------------------------------------->");
      radio.powerUp();
      uint32_t idTask = g_clipID*1000+task;
      uint32_t SendMsg[8] = {idTask, millis(), 0, 0, 0, 0, 0, 0};
      uint32_t RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
     // uint32_t timestamp = millis();

     // while (true){ ///     for (uint8_t i = 1; i < 255; i++ ) {
       for (uint8_t i = 1; i < 25; i++ ) { 
        Serial.print("ping. ");

         SendMsg[1] = millis();
         SendMsg[4] = analogRead(A7)*2*ARef/4096*1000; //Actual input voltage of Clip in mV. 4096 for 12-bits /1024 for 10-bits analog input
         
       ReadSensors(g_DataSensors);
       SendMsg[7] = g_DataSensors[4]*10000+g_DataSensors[5]; 

         // cout << endl << "Send Ping: " << SendMsg[0] << "," <<  SendMsg[1] << "," << SendMsg[2] << "," << SendMsg[3] << "," << SendMsg[4] << "," << SendMsg[5] << "," << SendMsg[6] << "," << SendMsg[7] << endl; 
          radio.write(&SendMsg, sizeof(SendMsg));
          
          if ( radio.isAckPayloadAvailable() ) {
              radio.read(&RcvMsg, sizeof(RcvMsg));
                      Serial.println("HAVE answer for ping. ");

            
          //   cout <<"Recv: " << RcvMsg[0] << "," << RcvMsg[1] << "," << RcvMsg[2] << "," << RcvMsg[3] << "," << RcvMsg[4] << "," << RcvMsg[5] << "," << RcvMsg[6] << "," << RcvMsg[7] << endl; 
//
//
//               // Calculate the Task
               uint32_t task = RcvMsg[0]%1000;
//               cout << "task :" << task << " ";
//               //Calculate the ID
               uint32_t id = (RcvMsg[0]-task)/1000;
//               cout << "id :" << id << " ";               
//               // Calculate the Ping
               uint32_t ping = SendMsg[1] - RcvMsg[1];
//               cout << "ping :" << ping << endl;

               

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
         else {
              Serial.println("NO answer for ping. ");
         // delay(5);
          
         } // end else


      } // end for 1 to
  radio.powerDown();
  } //end ping

