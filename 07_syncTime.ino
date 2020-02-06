/*=========================================================================
                         synctime()
    -----------------------------------------------------------------------*/
void synctime(int newTime) {
  DateTime now = rtc.now();
  Serial.print("Time Update old: "); Serial.println(now.unixtime());
  rtc.adjust(newTime);
  Serial.print("Time Update new: "); Serial.println(now.unixtime());
}
