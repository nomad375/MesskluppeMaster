/*=========================================================================
                         synctime()
    -----------------------------------------------------------------------*/
int synctime(int newTime){
    DateTime now = rtc.now();
    Serial.print("Time Update old: ");Serial.println(now.unixtime());
    rtc.adjust(newTime);

    Serial.print("Time Update new: ");Serial.println(now.unixtime());
    
 
   // if (AckPayload[3] != now.unixtime()){ //take care that inside AckPayload[3] yoy already have RPIs unix time.
    //  rtc.adjust(AckPayload[3]);
    //}


  
}
