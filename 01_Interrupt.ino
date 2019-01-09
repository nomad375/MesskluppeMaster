/*======================================================================================
        Interrupt
  ======================================================================================== */

void IRQ1() {
  noInterrupts();
  IamAtInlet = true;
  // Serial.print("<<--INLET--IRQ1>");
  // Serial.println(IamAtInlet);
  interrupts();

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
  g_YawOffset = imu.yaw - 90;
  //Serial.print("<<--Yaw Offset --> " );
  //Serial.println(g_YawOffset);
  // Serial.print("<<--OVEN--IRQ2> " );
  // Serial.println(IamInOven);
  // Serial.print("<<--INLET--IRQ2> " );
  // Serial.println(IamAtInlet);
  interrupts();
}
