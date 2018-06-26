/*======================================================================================
 *      Start measurement CSV
======================================================================================== */
void StartMesurmentCSV() {

 // prepare all variables
  uint32_t startTime = millis();          // variables for time in ms
  uint32_t actTime = startTime;
  uint32_t LineNumber = 0;
  boolean measurement_started = false;

  // Create the Filename
  DateTime now = rtc.now();
  char fN[18];
  sprintf(fN, "%04u_%2u%02u%02u_%02u%02u%02u.csv", g_clipID, now.year()-2000, now.month(), now.day(), now.hour(), now.minute(), now.second());
  
  // Open the file
  logfile.open(fN);         
  if (!logfile.is_open()) {
    digitalWrite(8, HIGH);
    error("file.open");                               //if file not open -> error
  }
  // Format the buffer
  obufstream bout(buf, sizeof(buf));
  
  // Create the header of the CSV File
    bout << F("timestamp; millis(); line; S1; S2; S3; S4; S5; S6;") << endl;
    logfile << buf;

  // Log data until timeout or interrupt
  while(actTime-startTime < g_maxMeasurement){
    if (IamInOven && !(actTime % g_logInterval)) {      
      measurement_started = true;             // keep it
      int32_t v1 = int(random(5000));
      int32_t v2 = int(random(5000));
      int32_t v3 = int(random(5000));
      int32_t v4 = int(random(5000));
      int32_t v5 = int(random(5000));
      int32_t v6 = int(random(5000));
      int32_t t  = actTime-startTime;
      
      obufstream bout(buf, sizeof(buf));

      DateTime now = rtc.now();
      bout << now.unixtime() << "; ";
      bout << t << "; ";
      bout << LineNumber << "; ";
      bout << v1 << "; ";
      bout << v2 << "; ";
      bout << v3 << "; ";
      bout << v4 << "; ";
      bout << v5 << "; ";
      bout << v6 << "; ";
      bout << endl;

      logfile << buf;
      //Log to Console
      //cout << buf;

      LineNumber ++; 
    } 
    if (!IamInOven && measurement_started) {
      Serial.println("Measurement finished!");
      break;
    } 
    
    // New Timer
    actTime = millis();
    delay (1);
  }
  logfile.close();
}


/*=========================================================================
                         GetRadioConnection()
    -----------------------------------------------------------------------*/
void GetRadioConnection()
{ //Start SendData() - based on fgets example from SdFat library

  radio.powerUp();
  digitalWrite(8, HIGH);

  uint32_t startMicros, actualMicros;
  startMicros = micros();
  actualMicros = micros();
  uint32_t counter = 0;

  uint32_t SendMessage[8] = {0};
  SendMessage[0] = UINT32_MAX;
  uint32_t AckPayload[4] = {0};

  Serial.println ("Establing conection... " );

  delay(250);
  while ( radio.write(&SendMessage, sizeof(SendMessage))) {


    //for (byte i = 0; i<8; i++){ Serial.print (SendMessage[i]); Serial.print(", "); } Serial.println (" ");


    if ( radio.isAckPayloadAvailable() ) {

      radio.read(&AckPayload, sizeof(AckPayload));
      Serial.print("  Ack we GOT: "); for (byte i = 0; i<4; i++)  {Serial.print (AckPayload[i]); Serial.print(", "); } Serial.println (" ");
      Serial.print("Micros per turn: "); Serial.println ((micros() - AckPayload[1]));

 
      counter ++;
      actualMicros = micros(); //update time
      delay (199 - counter * 2); //simulation of moving Clips to RPI reciever
      
/* Update time from RPI if differnce > 1 sec */

 DateTime now = rtc.now();
 
 if (AckPayload[3] != now.unixtime()){ //take care that inside AckPayload[3] yoy already have RPIs unix time.
 rtc.adjust(AckPayload[3]);
  
  } 


/* END Update time from RPI if differnce > 1 sec */

      


      SendMessage[0] = UINT32_MAX;
      SendMessage[1] = actualMicros;
      SendMessage[2] =  now.unixtime();
      SendMessage[3] =  33333333;
      SendMessage[4] =  44444444;
      SendMessage[5] =  55555555;
      SendMessage[6] =  66666666;
      SendMessage[7] =  77777777;

      //   SendMessage[2] = now.unixtime();



      if ((micros() - AckPayload[1]) / 1000 <= 70) {
        break;
      }

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

/*======================================================================================
 *      Reboot Arduino Zero
======================================================================================== */

//NVIC_SystemReset(); //reset CPU function

