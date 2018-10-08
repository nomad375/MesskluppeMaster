

/*=========================================================================
                         GetRadioConnection()
    -----------------------------------------------------------------------*/
void GetRadioConnection()
{ //S

  radio.powerUp();
  digitalWrite(8, HIGH);

  uint32_t startMicros, actualMicros;
  startMicros = micros();
  actualMicros = micros();
  uint32_t counter = 0;

  uint32_t SendMessage[8] = {0,0,0,0,0,0,0,0};
  SendMessage[0] = UINT32_MAX;
  uint32_t AckPayload[4] = {0};

  Serial.println ("Establing conection... " );

  delay(250);
  while ( radio.write(&SendMessage, sizeof(SendMessage))) {


    for (byte i = 0; i<8; i++){ Serial.print (SendMessage[i]); Serial.print(", "); } Serial.println (" ");


    if ( radio.isAckPayloadAvailable() ) {

      radio.read(&AckPayload, sizeof(AckPayload));
      Serial.print("  Ack we GOT: "); for (byte i = 0; i<4; i++)  {Serial.print (AckPayload[i]); Serial.print(", "); } Serial.println (" ");
      Serial.print("Micros per turn: "); Serial.println ((micros() - AckPayload[1]));

 
      counter ++;
      actualMicros = micros(); //update time
  //    delay (199 - counter * 2); //simulation of moving Clips to RPI reciever
      


 DateTime now = rtc.now();



      


      SendMessage[0] = UINT32_MAX;
      SendMessage[1] = actualMicros;
      SendMessage[2] =  now.unixtime();
      SendMessage[3] =  33333333;
      SendMessage[4] =  44444444;
      SendMessage[5] =  55555555;
      SendMessage[6] =  66666666;
      SendMessage[7] =  77777777;

      //   SendMessage[2] = now.unixtime();



   //   if ((micros() - AckPayload[1]) / 1000 <= 70) {  break;  }

    }
    else {
      Serial.println("  Acknowledge but no data ");
    }
    if (IamInOven == true) { // Stop ping if too late
      break;
    }

  }// end while ( radio.write


  Serial.print("Ping fineshed in "); Serial.print ((micros() - startMicros) ); Serial.println(" micros.");

  radio.powerDown();
  digitalWrite(8, LOW);
}// GetRadioConnection()


/*=========================================================================
                          Check RTC state
    -----------------------------------------------------------------------*/

void rtcPrint () { //start print of RTC state
  char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

//  Serial.print(" since midnight 1/1/1970 = ");
//  Serial.print(now.unixtime());
//  Serial.print("s = ");
 // Serial.print(now.unixtime() / 86400L);
//  Serial.println("d");
}// ent of RTCPrint ()


//end of file



