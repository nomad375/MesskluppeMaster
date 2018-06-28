/*=========================================================================
                         SendOnline()
    -----------------------------------------------------------------------*/
void SendOnline()
{ //SendOnline()

  radio.powerUp();

  uint32_t StartTime =millis();
  uint32_t LineNumber = 0;
  uint32_t SendMessage[8] = {0,0,0,0,0,0,0,0};
  SendMessage[0] = g_clipID*1000+30; // 30(!?!)= Online mode for RPI
  uint32_t RecievedMessage[8] = {0,0,0,0,0,0,0,0};

  Serial.println ("Online transfering... " );

  delay(250);
RecievedMessage[0]=30;

Serial.println ("Line to send "); for (byte i = 0; i<8; i++){ Serial.print (SendMessage[i]); Serial.print(", "); } Serial.println (" ");

while ((RecievedMessage[0]%1000 == 30) || (millis() - StartTime < 3000) ) { //run 30 seconds in any case, and then untill task changed (minimum 3 seconds)
radio.write(&SendMessage, sizeof(SendMessage));
          if ( radio.isAckPayloadAvailable() ) {
          
                radio.read(&RecievedMessage, sizeof(RecievedMessage));
                Serial.print("  Ack we GOT: "); for (byte i = 0; i<4; i++)  {Serial.print (RecievedMessage[i]); Serial.print(", "); } Serial.println("  ");
               
          
          //      while (actTime % g_logInterval);    // wait for time to be a multiple of interval
          
                DateTime now = rtc.now();
                LineNumber ++;
                SendMessage[0] = g_clipID*1000+00; //изменить идентификатор в1010 RPI
                SendMessage[1] = now.unixtime();
                SendMessage[2] = millis()%1000;
                SendMessage[3] = LineNumber;
                for (uint16_t ia = 1; ia <= 4; ia++) { SendMessage[ia+3]= analogRead(ia);  }  // 4 analog inputs to read values

                
              } //enf If radio.isAckPayloadAvailable() ) {

          else {  Serial.println("  Acknowledge but no data ");   }


  }// end while ( radio.write



  radio.powerDown();
 
}// GetRadioConnection()

