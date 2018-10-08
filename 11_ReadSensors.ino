
/*=========================================================================
    Analog read filtered
    -----------------------------------------------------------------------

void ReadSensors (uint16_t *g_DataSensors)
{
  uint32_t StartTime = micros();

//                   Cillect time stamp                                                                    
//                   Remember - last sensor reads ~1-3millisecond later then fist                         
g_DataSensors[0] = millis() %1000; //timing 
  //                   digital sensors read                                                                 
  if ( imu.dataReady() ) // think to add here RESPONSIVE_ANALOG_READ filter...
  { imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS | UPDATE_TEMP);

      g_DataSensors[4] = (uint16_t) imu.ax;
      g_DataSensors[5] = (uint16_t) imu.ay;
      g_DataSensors[6] = (uint16_t) imu.az;
      g_DataSensors[7] = (uint16_t) imu.gx;
      g_DataSensors[8] = (uint16_t) imu.gy;
      g_DataSensors[9] = (uint16_t) imu.gz;
     
      g_DataSensors[10] = (uint16_t) (imu.temperature/1000); // board temperature in C * 100

      g_DataSensors[12] = (uint16_t) imu.mx;
      g_DataSensors[13] = (uint16_t) imu.my;
      g_DataSensors[14] = (uint16_t) imu.mz;
  } //end if imu.dataReady()

   

  //                  analog sensors read using options from setup                         //
  if (RESPONSIVE_ANALOG_READ == 1) {

    if (ANALOG_READ_FAST == 1) {
      analog1.update(analogReadFast(A1));
      analog2.update(analogReadFast(A2));
      analog3.update(analogReadFast(A3));
      analog4.update(analogReadFast(A4));
      analog7.update(analogReadFast(A7));


    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)

      analog1.update();
      analog2.update();
      analog3.update();
      analog4.update();
      analog7.update();
    } //end else (ANALOG_READ_FAST == 0)

    g_DataSensors[1] = analog1.getValue();      // load cell X
    g_DataSensors[2] = analog2.getValue();      // load cell Y
    g_DataSensors[3] = analog3.getValue();      // load cell Z
    g_DataSensors[11] = analog4.getValue();     // Clip Temperature
    g_DataSensors[15] = analog7.getValue();     // internal voltage


  } //enf if (RESPONSIVE_ANALOG_READ ==1)


  else //if (RESPONSIVE_ANALOG_READ ==0)
  {

    if (ANALOG_READ_FAST == 1) {
      g_DataSensors[1] = analogReadFast(A1);
      g_DataSensors[2] = analogReadFast(A2);
      g_DataSensors[3] = analogReadFast(A3);
      g_DataSensors[11] = analogReadFast(A4);
      g_DataSensors[15] = analogReadFast(A7);
    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)
      g_DataSensors[1] = analogRead(A1);
      g_DataSensors[2] = analogRead(A2);
      g_DataSensors[3] = analogRead(A3);
      g_DataSensors[11] = analogRead(A4);
      g_DataSensors[15] = analogRead(A7);
    } //end if (ANALOG_READ_FAST == 1)

  } //(RESPONSIVE_ANALOG_READ ==0)

 



  //  Serial.print ("ANALOG_READ_FAST = "); Serial.println (ANALOG_READ_FAST);
  //  Serial.print ("RESPONSIVE_ANALOG_READ = "); Serial.println (RESPONSIVE_ANALOG_READ);
  //  uint32_t FinishTime = micros();
  //  Serial.println ( FinishTime - StartTime);

//Serial.print("g_DataSensors array     "); for (int i = 0; i < 16; i++) { Serial.print((int16_t)g_DataSensors[i]); Serial.print("  ");  }  Serial.println();
//Serial.print("g_DataSensors array HEX "); for (int i = 0; i < 16; i++) { Serial.print(g_DataSensors[i],HEX); Serial.print("  ");  }  Serial.println();


  // print for serial monitor..//

  //for (int i = 1; i <= 3; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");       //Load cells prinout
  //for (int i = 4; i <= 6; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");}  Serial.print (35000); Serial.print("  "); Serial.print (-35000); Serial.println("  ");         //Accelerometr printouts
  //for (int i = 7; i <= 9; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");       //Gyro printouts
//for (int i = 12; i <= 14; i++) {Serial.print((int16_t)g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");     //compass printouts
 //Serial.print(g_DataSensors[10]); Serial.println("  ");     //temperature printouts
 // Serial.print(imu.temperature); Serial.println("  ");   


}//end ReadSensors

/* ==========================================
        Print sensor values
  ======================================== */
void printSensorsDataCoverted(void)
{
//  // After calling update() the ax, ay, az, gx, gy, gz, mx,
//  // my, mz, time, and/or temerature class variables are all
//  // updated. Access them by placing the object. in front:
//
//  // Use the calcAccel, calcGyro, and calcMag functions to
//  // convert the raw sensor readings (signed 16-bit values)
//  // to their respective units.
//  float accelX = imu.calcAccel(imu.ax);
//  float accelY = imu.calcAccel(imu.ay);
//  float accelZ = imu.calcAccel(imu.az);
//  float gyroX = imu.calcGyro(imu.gx);
//  float gyroY = imu.calcGyro(imu.gy);
//  float gyroZ = imu.calcGyro(imu.gz);
//  float magX = imu.calcMag(imu.mx);
//  float magY = imu.calcMag(imu.my);
//  float magZ = imu.calcMag(imu.mz);
//  float Temperature = float(imu.temperature) / 100000;
//
//  //  Serial.println (WhereAmI);
//
//  //Serial.println("Accel: " + String(accelX) + ", " + String(accelY) + ", " + String(accelZ) + " g");
//  //Serial.println("Accel: " + String((imu.ax)) + ", " +  String((imu.ay)) + ", " + String((imu.az)) + " raw");
//  //Serial.println("Gyro: " + String(gyroX) + ", " + String(gyroY) + ", " + String(gyroZ) + " dps");
//  //Serial.println("Gyro: " + String((imu.gx)) + ", " +  String((imu.gy)) + ", " + String((imu.gz)) + " raw");
//  //Serial.println("Mag: " + String(magX) + ", " +  String(magY) + ", " + String(magZ) + " uT");
//  //Serial.println("Mag: " + String((imu.mx)) + ", " +  String((imu.my)) + ", " + String((imu.mz)) + " raw");
//  //Serial.println("Temperture: " + String(Temperature) + " gC");
//  Serial.println("Time IMU: " + String(imu.time) + " ms");
//  Serial.println("Time CPU: " + String(millis()) + " ms");
//
//  Serial.println();
} //end printIMUData(void)

/* ==========================================
    Setup for all sensonrs (analog + digial)
  ======================================== //

void SetupSensors ()
{

  analog1.setSnapMultiplier(0.01); // 0.01 by default
  analog2.setSnapMultiplier(0.01); // 0.01 by default
  analog3.setSnapMultiplier(0.01); // 0.01 by default
  analog4.setSnapMultiplier(0.01); // 0.01 by default
  analog7.setSnapMultiplier(0.01); // 0.01 by default

  analog1.setAnalogResolution(4096);
  analog2.setAnalogResolution(4096);
  analog3.setAnalogResolution(4096);
  analog4.setAnalogResolution(4096);
  analog7.setAnalogResolution(4096);


  analog1.enableSleep();
  analog2.enableSleep();
  analog3.enableSleep();
  analog4.enableSleep();
  analog7.enableSleep();

  analog1.enableEdgeSnap();
  analog2.enableEdgeSnap();
  analog3.enableEdgeSnap();
  analog4.enableEdgeSnap();
  analog7.enableEdgeSnap();

  //             Setup of I2C Accelerometer/gyro/compas sensor  //

  // Call imu.begin() to verify communication with and
  // initialize the MPU-9250 to it's default values.
  // Most functions return an error code - INV_SUCCESS (0)
  // indicates the IMU was present and successfully set up
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  imu.setGyroFSR(1000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  imu.setAccelFSR(2); // Set accel to +/-2g
  // Note: the MPU-9250's magnetometer FSR is set at
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  imu.setLPF(20); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  imu.setSampleRate(400); // Set sample rate to 10Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate(100); // Set mag rate to 10Hz



} // end SetupSensors ()
*/
/* ==========================================
    Setup for all sensonrs (analog + digial)
  ======================================== */

void SetupSensorsTEMP ()
{

  analog1.setSnapMultiplier(0.01); // 0.01 by default
  analog2.setSnapMultiplier(0.01); // 0.01 by default
  analog3.setSnapMultiplier(0.01); // 0.01 by default
  analog4.setSnapMultiplier(0.01); // 0.01 by default
  analog7.setSnapMultiplier(0.01); // 0.01 by default

  analog1.setAnalogResolution(4096);
  analog2.setAnalogResolution(4096);
  analog3.setAnalogResolution(4096);
  analog4.setAnalogResolution(4096);
  analog7.setAnalogResolution(4096);


  analog1.enableSleep();
  analog2.enableSleep();
  analog3.enableSleep();
  analog4.enableSleep();
  analog7.enableSleep();

  analog1.enableEdgeSnap();
  analog2.enableEdgeSnap();
  analog3.enableEdgeSnap();
  analog4.enableEdgeSnap();
  analog7.enableEdgeSnap();

  /*
              Setup of I2C Accelerometer/gyro/compas sensor

  */

  imu.ax = 1111;
  imu.ay = 2222;
  imu.az = 3333;
  imu.gx = 4444;
  imu.gy = 5555;
  imu.gz = 6666;
  imu.temperature = (uint16_t) -9999; // board temperature in C * 100
  imu.mx = 7777;
  imu.my = 8888;
  imu.mz = 9999;

} // end SetupSensorsTMP ()


/* Tead Sensors TMP */

void ReadSensorsTMP (uint16_t *g_DataSensors)
{
  uint32_t StartTime = micros();

/*                   Cillect time stamp                                                                     */
/*                   Remember - last sensor reads ~1-3millisecond later then fist                          */
g_DataSensors[0] = millis() %1000; //timing 
  /*                   digital sensors read                                                                  */
 

      g_DataSensors[4] = (uint16_t) imu.ax;
      g_DataSensors[5] = (uint16_t) imu.ay;
      g_DataSensors[6] = (uint16_t) imu.az;
      g_DataSensors[7] = (uint16_t) imu.gx;
      g_DataSensors[8] = (uint16_t) imu.gy;
      g_DataSensors[9] = (uint16_t) imu.gz;
     
      g_DataSensors[10] = (uint16_t) (imu.temperature/1000); // board temperature in C * 100

      g_DataSensors[12] = (uint16_t) imu.mx;
      g_DataSensors[13] = (uint16_t) imu.my;
      g_DataSensors[14] = (uint16_t) imu.mz;


   

  /*                   analog sensors read using options from setup                         */
  if (RESPONSIVE_ANALOG_READ == 1) {

    if (ANALOG_READ_FAST == 1) {
      analog1.update(analogReadFast(A1));
      analog2.update(analogReadFast(A2));
      analog3.update(analogReadFast(A3));
      analog4.update(analogReadFast(A4));
      analog7.update(analogReadFast(A7));


    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)

      analog1.update();
      analog2.update();
      analog3.update();
      analog4.update();
      analog7.update();
    } //end else (ANALOG_READ_FAST == 0)

    g_DataSensors[1] = analog1.getValue();      // load cell X
    g_DataSensors[2] = analog2.getValue();      // load cell Y
    g_DataSensors[3] = analog3.getValue();      // load cell Z
    g_DataSensors[11] = analog4.getValue();     // Clip Temperature
    g_DataSensors[15] = analog7.getValue();     // internal voltage


  } //enf if (RESPONSIVE_ANALOG_READ ==1)


  else //if (RESPONSIVE_ANALOG_READ ==0)
  {

    if (ANALOG_READ_FAST == 1) {
      g_DataSensors[1] = analogReadFast(A1);
      g_DataSensors[2] = analogReadFast(A2);
      g_DataSensors[3] = analogReadFast(A3);
      g_DataSensors[11] = analogReadFast(A4);
      g_DataSensors[15] = analogReadFast(A7);
    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)
      g_DataSensors[1] = analogRead(A1);
      g_DataSensors[2] = analogRead(A2);
      g_DataSensors[3] = analogRead(A3);
      g_DataSensors[11] = analogRead(A4);
      g_DataSensors[15] = analogRead(A7);
    } //end if (ANALOG_READ_FAST == 1)

  } //(RESPONSIVE_ANALOG_READ ==0)

 



  //  Serial.print ("ANALOG_READ_FAST = "); Serial.println (ANALOG_READ_FAST);
  //  Serial.print ("RESPONSIVE_ANALOG_READ = "); Serial.println (RESPONSIVE_ANALOG_READ);
  //  uint32_t FinishTime = micros();
  //  Serial.println ( FinishTime - StartTime);

//Serial.print("g_DataSensors array     "); for (int i = 0; i < 16; i++) { Serial.print((int16_t)g_DataSensors[i]); Serial.print("  ");  }  Serial.println();
//Serial.print("g_DataSensors array HEX "); for (int i = 0; i < 16; i++) { Serial.print(g_DataSensors[i],HEX); Serial.print("  ");  }  Serial.println();


  /* print for serial monitor..*/

  //for (int i = 1; i <= 3; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");       //Load cells prinout
  //for (int i = 4; i <= 6; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");}  Serial.print (35000); Serial.print("  "); Serial.print (-35000); Serial.println("  ");         //Accelerometr printouts
  //for (int i = 7; i <= 9; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");       //Gyro printouts
//for (int i = 12; i <= 14; i++) {Serial.print((int16_t)g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");     //compass printouts
 //Serial.print(g_DataSensors[10]); Serial.println("  ");     //temperature printouts
 // Serial.print(imu.temperature); Serial.println("  ");   


}//end ReadSensors


