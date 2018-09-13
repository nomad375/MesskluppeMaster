
/*=========================================================================
    Analog read filtered
    -----------------------------------------------------------------------*/

void ReadSensors (uint32_t *g_DataSensors)
{

//uint32_t StartTime = micros();

  analog1.setSnapMultiplier(0.01); // 0.01 by default
  analog2.setSnapMultiplier(0.01); // 0.01 by default
  analog3.setSnapMultiplier(0.01); // 0.01 by default
analog4.setSnapMultiplier(0.01); // 0.01 by default
analog5.setSnapMultiplier(0.01); // 0.01 by default


  analog1.setAnalogResolution(4096);
  analog2.setAnalogResolution(4096);
  analog3.setAnalogResolution(4096);
 analog4.setAnalogResolution(1024);
 analog5.setAnalogResolution(1024);

  analog1.enableSleep();
  analog2.enableSleep();
  analog3.enableSleep();
 analog4.enableSleep();
 analog5.enableSleep();

  analog1.enableEdgeSnap();
  analog2.enableEdgeSnap();
  analog3.enableEdgeSnap();
 analog4.enableEdgeSnap();
 analog5.enableEdgeSnap();

  analog1.update(); g_DataSensors[1] = analog1.getValue() * ARef / 4096 * 1000;
  analog2.update(); g_DataSensors[2] = analog2.getValue() * ARef / 4096 * 1000;
  analog3.update(); g_DataSensors[3] = analog3.getValue() * ARef / 4096 * 1000;
 analog4.update(); g_DataSensors[4] = analog4.getValue();// * ARef / 4096 * 1000;
 analog5.update(); g_DataSensors[5] = analog5.getValue();// * ARef / 4096 * 1000;



//Serial.println ( micros()- StartTime);

}//end ReadSensors

