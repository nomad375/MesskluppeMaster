/*======================================================================================
        Interrupt
  ======================================================================================== */

void IRQ1() {
  noInterrupts();
  IamAtInlet = true;
Serial.print("<<--INLET--IRQ1>"); Serial.println(IamAtInlet);
  interrupts();

}

void IRQ2() {
  noInterrupts();

  if (IamAtInlet == true) {
        IamAtInlet = false;

                g_YawOffset = imu.yaw - 90;
        IamInOven = true;

        Serial.print("<<--Yaw Offset --> " ); Serial.println(g_YawOffset);

  }


 else if (IamAtInlet == false && (millis()-6000) > IRQ2_time) {
   IamInOven = false;
 }


IRQ2_time = millis();
  Serial.print("<<--OVEN--IRQ2> " ); Serial.println(IamInOven);
  


 interrupts();
  
}
