/* ==========================================
    Setup for all sensonrs (analog + digial)
  ======================================== */

void SetupSensors ()
{

  analog1.setSnapMultiplier(0.01); // 0.01 by default
  analog2.setSnapMultiplier(0.01); // 0.01 by default
  analog3.setSnapMultiplier(0.01); // 0.01 by default
  analog4.setSnapMultiplier(0.01); // 0.01 by default
  analog5.setSnapMultiplier(0.01); // 0.01 by default

  analog1.setAnalogResolution(4096);
  analog2.setAnalogResolution(4096);
  analog3.setAnalogResolution(4096);
  analog4.setAnalogResolution(4096);
  analog5.setAnalogResolution(4096);


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

/*=========================================================================
    Analog read filtered
    ----------------------------------------------------------------------- */

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
     
      g_DataSensors[10] = (uint16_t) (imu.temperature/1000-290); // board temperature in C * 100 (-offset of Almemo)

      g_DataSensors[12] = (uint16_t) imu.mx;
      g_DataSensors[13] = (uint16_t) imu.my;
      g_DataSensors[14] = (uint16_t) imu.mz;
  } //end if imu.dataReady()

<<<<<<< HEAD
  if ( imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
      Yaw = imu.yaw - g_YawOffset;
  
      if (Yaw >=360){NewYaw = Yaw-360;}
      else if (Yaw <0){NewYaw = 360 + Yaw;}
      else { NewYaw = Yaw;}

      // g_DataSensors[4] = (uint16_t) (imu.calcAccel(imu.ax) * 1000);  // check what position is better for accererometer reading. here at dmpUpdateFifo or above at  imu.update
      // g_DataSensors[5] = (uint16_t) (imu.calcAccel(imu.ay) * 1000);
      g_DataSensors[6] =  (uint16_t) NewYaw;

    }
  }//end if imu.fifoAvailable()



=======
   
>>>>>>> parent of 94290d9... Clean Up

  //                  analog sensors read using options from setup                         //
  
  if (RESPONSIVE_ANALOG_READ == 1) {

    if (ANALOG_READ_FAST == 1) {
      analog1.update(analogReadFast(A1));
      analog2.update(analogReadFast(A2));
      analog3.update(analogReadFast(A3));
      analog4.update(analogReadFast(A4)); //Serial.print ((Tclip*g_ARef/4096-0.463)*100); Serial.print (" C ");
      analog5.update(analogReadFast(A5)); 
    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)

      analog1.update();
      analog2.update();
      analog3.update();
      analog4.update();
      analog5.update();
    } //end else (ANALOG_READ_FAST == 0)

    g_DataSensors[1] = analog1.getValue();      // load cell X
    g_DataSensors[2] = analog2.getValue();      // load cell Y
    g_DataSensors[3] = analog3.getValue();      // load cell Z
    g_DataSensors[11] = analog4.getValue();     // Clip Temperature
    g_DataSensors[15] = analog5.getValue();     // internal voltage


  } //enf if (RESPONSIVE_ANALOG_READ ==1)


  else //if (RESPONSIVE_ANALOG_READ ==0)
  {

    if (ANALOG_READ_FAST == 1) {
      g_DataSensors[1] = analogReadFast(A1);   
      g_DataSensors[2] = analogReadFast(A2);
      g_DataSensors[3] = analogReadFast(A3);
      g_DataSensors[11] = analogReadFast(A4);
      g_DataSensors[15] = analogReadFast(A5);
    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)
      g_DataSensors[1] = analogRead(A1);
      g_DataSensors[2] = analogRead(A2);
      g_DataSensors[3] = analogRead(A3);
      g_DataSensors[11] = analogRead(A4);
      g_DataSensors[15] = analogRead(A5);
    } //end if (ANALOG_READ_FAST == 1)

  } //(RESPONSIVE_ANALOG_READ ==0)

 
//  Serial.print ("ANALOG_READ_FAST = "); Serial.println (ANALOG_READ_FAST);
//  Serial.print ("RESPONSIVE_ANALOG_READ = "); Serial.println (RESPONSIVE_ANALOG_READ);
//  uint32_t FinishTime = micros();
//  Serial.println ( FinishTime - StartTime);

//Serial.print("g_DataSensors array     "); for (int i = 0; i < 16; i++) { Serial.print((int16_t)g_DataSensors[i]); Serial.print("  ");  }  Serial.println();
//Serial.print("g_DataSensors array HEX "); for (int i = 0; i < 16; i++) { Serial.print(g_DataSensors[i],HEX); Serial.print("  ");  }  Serial.println();

<<<<<<< HEAD
  float ARef = 2.23; //Reference Voltage for analog inputs. Ised Internal AR_INTERNAL2V23

//  float mV_X = 2.461;
//  float mV_Y = 2.433;
//  float mV_Z = 2.429;

 float mV_X = 2.313; 
 float mV_Y = 2.323; 
 float mV_Z = 2.319; 
//
//  float mV_X =  (2.055);
//  float mV_Y = (2.055);
//  float mV_Z = (2.063);

  float Gain_X = 1805.801802; //(33.3 Ohm)
  float Gain_Y = 336.963374; //180.2 Ohm
  float Gain_Z = 404;  //150 Ohm
=======
>>>>>>> parent of 94290d9... Clean Up

  // print for serial monitor..//

<<<<<<< HEAD
  float Ufx, Ufy, Ufz;
//  float offsetX = 46;
//  float offsetY = -18;
//  float offsetZ = -31;
  float offsetX = 0;
  float offsetY = 0;
  float offsetZ = 0;

  Serial.print ("Raw X: "); Serial.print (RawX); Serial.print(" bit, ");
  Serial.print ("Raw Y: "); Serial.print (RawY); Serial.print(" bit, ");
  Serial.print ("Raw Z: "); Serial.print (RawZ); Serial.print(" bit, ");

  Ufx = RawX  * ARef / 4096 * 1000 / mV_X / Gain_X ;
  Ufy = RawY  * ARef / 4096 * 1000 / mV_Y / Gain_Y ;
  Ufz = RawZ  * ARef / 4096 * 1000 / mV_Z / Gain_Z ;

  float     ForceX = Ufx * K11 + Ufy * K12 + Ufz * K13 - offsetX;
  float     ForceY = Ufx * K21 + Ufy * K22 + Ufz * K23 - offsetY;
  float     ForceZ = Ufx * K31 + Ufy * K32 + Ufz * K33 - offsetZ;

  g_DataSensors[1] = (uint16_t)(ForceX);
  g_DataSensors[2] = (uint16_t)(ForceY);
  g_DataSensors[3] = (uint16_t)(ForceZ);
  g_DataSensors[8] = (uint16_t)(((RawTclip * ARef * 1000 / 4096) - 500) / 0.1); // [(analog voltage in mV) - 500] / 10
  g_DataSensors[9] = (uint16_t)(RawVbat * 2 * ARef / 4096 * 1000-70);
=======
 //for (int i = 1; i <= 3; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");       //Load cells prinout
 //for (int i = 4; i <= 6; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");}  Serial.print (35000); Serial.print("  "); Serial.print (-35000); Serial.println("  ");         //Accelerometr printouts
 //for (int i = 7; i <= 9; i++) {Serial.print(g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");       //Gyro printouts
 //for (int i = 12; i <= 14; i++) {Serial.print((int16_t)g_DataSensors[i]); Serial.print("  ");Serial.print (35000); Serial.print("  ");Serial.print (-35000); Serial.print("  ");}  Serial.println("  ");     //compass printouts
 //Serial.print(g_DataSensors[10]); Serial.println("  ");     //temperature printouts
 // Serial.print(imu.temperature); Serial.println("  ");   


}//end ReadSensors
>>>>>>> parent of 94290d9... Clean Up

/* ==========================================
        Print sensor values
  ======================================== */
void printSensorsDataConverted(void)
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

