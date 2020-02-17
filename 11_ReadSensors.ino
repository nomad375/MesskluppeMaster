/* ==========================================
    Setup for all sensonrs (analog + digial)
  ======================================== */

void SetupSensors (){
  

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
      delay(1000);
    }
  }

  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);

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
  //imu.setSampleRate(400); // USED before 2020 02 07 
  imu.setSampleRate(800); // Set sample rate to 800Hz 

  imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | // Enable 6-axis quat
               DMP_FEATURE_GYRO_CAL, // Use gyro calibration
               400); // Set DMP FIFO rate to 100! Hz
  // DMP_FEATURE_LP_QUAT can also be used. It uses the
  // accelerometer in low-power mode to estimate quat's.
  // DMP_FEATURE_LP_QUAT and 6X_LP_QUAT are mutually exclusive


} // end SetupSensors ()

/*=========================================================================
    Analog read filtered
    ----------------------------------------------------------------------- */

void ReadSensors (uint16_t *g_DataSensors)
{

  uint16_t RawX, RawY, RawZ, RawTclip, RawVbat;
  float Yaw, NewYaw;


  //  Collect time stamp
  //  Remember - last sensor reads ~1-3 millisecond later then fist
  g_DataSensors[0] = (uint16_t)(millis() % 1000UL); //timing

  //digital sensors read

  if ( imu.dataReady() ) // think to add here RESPONSIVE_ANALOG_READ filter...
  { imu.update(UPDATE_ACCEL | UPDATE_TEMP);

    g_DataSensors[4] = (uint16_t) (imu.calcAccel(imu.ax) * 1000);
    g_DataSensors[5] = (uint16_t) (imu.calcAccel(imu.ay) * 1000);
    g_DataSensors[7] = (uint16_t) (imu.temperature / 1000); // board temperature in C * 100 (-offset of Almemo)

  } //end if imu.dataReady()

  if ( imu.fifoAvailable() )
  {
    // Use dmpUpdateFifo to update the ax, gx, mx, etc. values
    if ( imu.dmpUpdateFifo() == INV_SUCCESS)
    {
      // computeEulerAngles can be used -- after updating the
      // quaternion values -- to estimate roll, pitch, and yaw
      imu.computeEulerAngles();
     
      Yaw = imu.yaw - g_YawOffset;
  
      if (Yaw >=360){NewYaw = Yaw-360.0;}
      else if (Yaw <0){NewYaw = Yaw+360.0;}
      else { NewYaw = Yaw;}

      g_DataSensors[6] =  (uint16_t) (NewYaw *10); // *10 to see decimal value;

    }
  }//end if imu.fifoAvailable()




  //                  analog sensors read using options from setup                         //

  if (RESPONSIVE_ANALOG_READ == 1) {

    if (ANALOG_READ_FAST == 1) {
      analog1.update(analogReadFast(A1));
      analog2.update(analogReadFast(A2));
      analog3.update(analogReadFast(A3));
      analog4.update(analogReadFast(A4)); 
      analog5.update(analogReadFast(A5));
    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)

      analog1.update();
      analog2.update();
      analog3.update();
      analog4.update();
      analog5.update();
    } //end else (ANALOG_READ_FAST == 0)

    RawX = analog1.getValue();      // load cell X RAW
    RawY = analog2.getValue();      // load cell Y RAW
    RawZ = analog3.getValue();      // load cell Z RAW
    RawTclip = analog4.getValue();  // Clip Temperature
    RawVbat = analog5.getValue();   // internal voltage


  } //enf if (RESPONSIVE_ANALOG_READ ==1)


  else //if (RESPONSIVE_ANALOG_READ ==0)
  {

    if (ANALOG_READ_FAST == 1) {
      RawX  = analogReadFast(A1);
      RawY  = analogReadFast(A2);
      RawZ  = analogReadFast(A3);
      RawTclip = analogReadFast(A4);
      RawVbat = analogReadFast(A5);
    } //end if (ANALOG_READ_FAST == 1)

    else { //( ANALOG_READ_FAST == 0)
      RawX  = analogRead(A1);
      RawY  = analogRead(A2);
      RawZ  = analogRead(A3);
      RawTclip = analogRead(A4);
      RawVbat = analogRead(A5);
    } //end if (ANALOG_READ_FAST == 1)

  } //(RESPONSIVE_ANALOG_READ ==0)



//  float ARef = 2.23; //Reference Voltage for analog inputs. Ised Internal AR_INTERNAL2V23

//Serial.print ("ZERO : "); Serial.print (0); Serial.print (" ");

Serial.print ("Yaw raw: "); Serial.print (imu.yaw); Serial.print(" raw, ");

Serial.print ("Yaw angle: "); Serial.print (g_DataSensors[6]/10); Serial.print(" degree, "); 

//Serial.print ("Raw X: "); Serial.print (RawX); Serial.print(" bit, ");
//Serial.print ("Raw Y: "); Serial.print (RawY); Serial.print(" bit, ");
//Serial.print ("Raw Z: "); Serial.print (RawZ); Serial.print(" bit, ");
//Serial.print ("Ti: "); Serial.print (g_DataSensors[7]/333.87f + 21.0f); Serial.print(" C, ");
//Serial.print ("Battery: "); Serial.print ((RawVbat) * 2.23 /4096*2); Serial.print(" V, ");
//Serial.print ("Battery: "); Serial.print (RawVbat); Serial.print(" bits, ");
//Serial.print ("MAX : "); Serial.print (4100);
Serial.println (" ");


    g_DataSensors[1] = (uint16_t)(RawX);
    g_DataSensors[2] = (uint16_t)(RawY);
    g_DataSensors[3] = (uint16_t)(RawZ);
    g_DataSensors[8] = (uint16_t)(RawTclip);
    g_DataSensors[9] = (uint16_t)(RawVbat);


    Watchdog.reset();

}//end ReadSensors
