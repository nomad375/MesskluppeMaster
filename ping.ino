/*=========================================================================
                         ping()
    -----------------------------------------------------------------------*/
    int ping (){
      // Prepare all variables
      uint32_t idTask = g_clipID*1000;
      uint32_t SendMsg[8] = {idTask, micros(), 0, 0, 0, 0, 0, 0};
      uint32_t timestamp = micros();
      uint32_t RcvMsg[8] = {0, 0, 0, 0, 0, 0, 0, 0};

      
      //cout << "g[0]: " << g_RcvMsg[0] << " g[1]: " << g_RcvMsg[1] << " g[2]: " << g_RcvMsg[2] << " g[3]: " << g_RcvMsg[3] << " g[4]: " << g_RcvMsg[4] << " g[5]: " << g_RcvMsg[5] << " g[6]: " << g_RcvMsg[6] << " g[7]: " << g_RcvMsg[7] << endl;
      
        radio.powerUp();
        // Loop try to ping max 20sec
        while ((micros()-timestamp) < (g_maxPing*2)){

          SendMsg[1] = micros();
          radio.write(&SendMsg, sizeof(SendMsg));
            
            if ( radio.isAckPayloadAvailable() ) {
              radio.read(&RcvMsg, sizeof(RcvMsg));
                   //Serial.print(" SendMsg[0]: ");Serial.print(SendMsg[0]); Serial.print(" SendMsg[1]: ");Serial.print(SendMsg[1]); Serial.print(" SendMsg[2]: ");Serial.print(SendMsg[2]); Serial.print(" SendMsg[3]: ");Serial.print(SendMsg[3]); Serial.print(" SendMsg[4]: ");Serial.print(SendMsg[4]);Serial.print(" SendMsg[5]: ");Serial.print(SendMsg[5]);Serial.print(" SendMsg[6]: ");Serial.print(SendMsg[6]); Serial.print(" SendMsg[7]: ");Serial.println(SendMsg[7]);
                   //Serial.print(" RcvMsg[0] : ");Serial.print(RcvMsg[0]);Serial.print(" RcvMsg[1] : ");Serial.print(RcvMsg[1]);Serial.print(" RcvMsg[2] : ");Serial.print(RcvMsg[2]);Serial.print(" RcvMsg[3] : ");Serial.print(RcvMsg[3]); Serial.print(" RcvMsg[4] : ");Serial.print(RcvMsg[4]);Serial.print(" RcvMsg[5] : ");Serial.print(RcvMsg[5]);Serial.print(" RcvMsg[6] : ");Serial.print(RcvMsg[6]);Serial.print(" RcvMsg[7] : ");Serial.println(RcvMsg[7]);

              // Calculate the Ping
              uint32_t pingi = SendMsg[1] - RcvMsg[1];
              //Serial.println(pingi);
              // Calculate the Task
              uint32_t task = RcvMsg[0]%1000;
              //Serial.println(task);
              //Calculate the ID
             uint32_t id = (RcvMsg[0]-task)/1000;
              //Serial.println(id);
               
        
              // Check if Clip ID is the same and delay smaller then g_maxPing
              if (id == g_clipID && pingi < g_maxPing && pingi > 0){
                  // Save the RcvMsg in g_RcvMsg
                  g_RcvMsg[0] = RcvMsg[0];    // Clip idTask
                  g_RcvMsg[1] = RcvMsg[1];    // Ping Timestamp
                  g_RcvMsg[2] = RcvMsg[2];    
                  g_RcvMsg[3] = RcvMsg[3];    // Real Time
                  g_RcvMsg[4] = RcvMsg[4];
                  g_RcvMsg[5] = RcvMsg[5];
                  g_RcvMsg[6] = RcvMsg[6];
                  g_RcvMsg[7] = RcvMsg[7];

                  Serial.println(SendMsg[1] - RcvMsg[1]);
                  return (SendMsg[1] - RcvMsg[1]);                                   // Return the ping
              } else {
                Serial.println("bad Ping");
                  return -1;                                                         // Return not same clipID or ping to high
              }
            }else{
              //Serial.println("No Payload");
              return -2;                                                           // No AckPayload Available
            }
            
        }
    }
