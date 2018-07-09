/*=========================================================================
                         SendOnline()
    -----------------------------------------------------------------------*/
void SendOnline()
{ //SendOnline()

  radio.powerUp();

  uint32_t StartTime = millis();
  uint32_t LineNumber = 0;
  uint32_t SendMessage[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  SendMessage[0] = g_clipID * 1000 + 60; // 30(!?!)= Online mode for RPI
  uint32_t RecievedMessage[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  Serial.println ("Online transfering... " );

  delay(250);




  while ((RecievedMessage[0] % 1000 == 60) || (millis() - StartTime < 3000) ) { //run 3000 milliseconds in any case, and then untill task changed
    Serial.print ("Line to send: "); for (byte i = 0; i<8; i++){ Serial.print (SendMessage[i]); Serial.print(", "); } Serial.println (" ");
    radio.write(&SendMessage, sizeof(SendMessage));
    if ( radio.isAckPayloadAvailable() ) {
      radio.read(&RecievedMessage, sizeof(RecievedMessage));
      //  Serial.print("  Ack we GOT: "); for (byte i = 0; i<8; i++)  {Serial.print (RecievedMessage[i]); Serial.print(", "); } Serial.println("  ");


      //      while (actTime % g_logInterval);    // wait for time to be a multiple of interval

      DateTime now = rtc.now();
      LineNumber ++;
      SendMessage[0] = g_clipID * 1000 + 60;
      SendMessage[1] = now.unixtime();
      SendMessage[2] = millis() % 1000;
      SendMessage[3] = LineNumber;
      for (uint16_t ia = 1; ia <= 4; ia++) {
        SendMessage[ia + 3] = analogRead(ia);   // 4 analog inputs to read values
      }


    } //enf If radio.isAckPayloadAvailable() ) {

    else {
     // Serial.println("  Acknowledge but no data ");   
     Serial.print(".");
    }


  }// end while ( radio.write


  Serial.print("Online sending finish in "); Serial.print ((millis() - StartTime) / 1000); Serial.println(" seconds.");
  radio.powerDown();

}// GetRadioConnection()

