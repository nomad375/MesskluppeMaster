/*=========================================================================
                         synctime()
    -----------------------------------------------------------------------*/
int synctime(int actTime){
    DateTime now = rtc.now();
    Serial.println(now.unixtime());
    rtc.adjust(actTime);
    
 
   // if (AckPayload[3] != now.unixtime()){ //take care that inside AckPayload[3] yoy already have RPIs unix time.
    //  rtc.adjust(AckPayload[3]);
    //}


  
}
