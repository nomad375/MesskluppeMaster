/*======================================================================================
 *      Interrupt
======================================================================================== */

void IRQ1() {
  noInterrupts();
  IamAtInlet = true;
  interrupts();
  Serial.println("<<--INLET-->");
} 

void IRQ2() {
  noInterrupts();
  
  if (IamAtInlet == true) {
    IamInOven = true;
    IamAtInlet = false;
  }
  else  {
    IamInOven = false;
  }
<<<<<<< HEAD
  g_YawOffset = imu.yaw - 90;
  //Serial.print("<<--Yaw Offset --> " );
  //Serial.println(g_YawOffset);
  // Serial.print("<<--OVEN--IRQ2> " );
  // Serial.println(IamInOven);
  // Serial.print("<<--INLET--IRQ2> " );
  // Serial.println(IamAtInlet);
=======
  Serial.print("<<--OVEN--> " );
  Serial.println(IamInOven);
>>>>>>> parent of 94290d9... Clean Up
  interrupts();
}


