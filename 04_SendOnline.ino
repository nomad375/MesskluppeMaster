/*=========================================================================
                         SendOnline()
    -----------------------------------------------------------------------*/
void SendOnline(){ //SendOnline()

  radio.powerUp();

  uint32_t StartTime = millis();
  uint32_t LineNumber = 0;
  uint16_t SendMessage[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  SendMessage[0] = g_clipID * 1000 + 60; // 30(!?!)= Online mode for RPI
  uint32_t RecievedMessage[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint32_t actTime;

  while ((RecievedMessage[0] % 1000 == 60) || (millis() - StartTime < 7000) ) { //run 3000 milliseconds in any case, and then untill task changed

    do {
      actTime = millis();
    }
    while (actTime % (g_logInterval * 25));  // wait for time to be a multiple of interval *10

    //Serial.print ("Line to send Online: "); for (byte i = 0; i < 16; i++) {Serial.print ((int16_t)SendMessage[i]);Serial.print(", ");} Serial.println (" ");
    
    radio.write(&SendMessage, sizeof(SendMessage));
    
    if ( radio.isAckPayloadAvailable() ) {
      radio.read(&RecievedMessage, sizeof(RecievedMessage));
    //   Serial.print("  Ack we GOT: "); for (byte i = 0; i<16; i++)  {Serial.print (RecievedMessage[i]); Serial.print(", "); } Serial.println("  ");

      DateTime now = rtc.now();
      LineNumber ++;
      SendMessage[0] = g_clipID * 1000 + 60;
      
      uint32_t currenttime = millis();
      SendMessage[1] = (uint16_t) ((currenttime >> 16) & 0xFFFF) ;
      SendMessage[2] = (uint16_t) (currenttime & 0xFFFF) ;

      ReadSensors(g_DataSensors);

      for (uint16_t ia = 1; ia <= 9; ia++) {
        SendMessage[ia + 4] = g_DataSensors[ia];   // DataSensors 1-9
      }

    } //enf If radio.isAckPayloadAvailable() ) {

    else {
      //Serial.println("Send Online -   Acknowledge but no data ");
      //break;

    }


  }// end while ((RecievedMessage[0] % 1000 == 60)

 // Serial.print("Online sending finish in "); Serial.print ((millis() - StartTime) / 1000); Serial.println(" seconds.");
  radio.powerDown();
  

}// END of void SendOnline()
