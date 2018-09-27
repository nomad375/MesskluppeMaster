
/*=========================================================================
    Analog read filtered
    -----------------------------------------------------------------------*/

void ReadSensors (uint16_t *g_DataSensors)
{

//  uint32_t StartTime = micros();



  analog1.update(); g_DataSensors[1] = analog1.getValue() * g_AnalogToMV;      // load cell X
  analog2.update(); g_DataSensors[2] = analog2.getValue() * g_AnalogToMV;      // load cell Y
  analog3.update(); g_DataSensors[3] = analog3.getValue() * g_AnalogToMV;      // load cell Z
  analog4.update(); g_DataSensors[4] = analog4.getValue() * g_AnalogToMV;      // Clip Temperature
  //analog5.update(); g_DataSensors[5] = analog5.getValue() * g_AnalogToMV;      // Board Temperature
  //g_DataSensors[4] = analogRead(A4) * g_AnalogToMV;      // Clip Temperature


//uint32_t FinishTime = micros();

//  Serial.println ( FinishTime - StartTime);

}//end ReadSensors


void SetupSensors ()
{

  analog1.setSnapMultiplier(0.01); // 0.01 by default
  analog2.setSnapMultiplier(0.01); // 0.01 by default
  analog3.setSnapMultiplier(0.01); // 0.01 by default
  analog4.setSnapMultiplier(0.01); // 0.01 by default
//  analog5.setSnapMultiplier(0.01); // 0.01 by default

  analog1.setAnalogResolution(4096);
  analog2.setAnalogResolution(4096);
  analog3.setAnalogResolution(4096);
  analog4.setAnalogResolution(1024);
//  analog5.setAnalogResolution(1024);

  analog1.enableSleep();
  analog2.enableSleep();
  analog3.enableSleep();
  analog4.enableSleep();
//  analog5.enableSleep();

  analog1.enableEdgeSnap();
  analog2.enableEdgeSnap();
  analog3.enableEdgeSnap();
  analog4.enableEdgeSnap();
//  analog5.enableEdgeSnap();

/*
 *          Setup of I2C connected Accelerometer/gyro/compas sensor
 * 
 */




} // end SetupSensors ()

