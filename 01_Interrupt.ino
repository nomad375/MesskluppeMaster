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
  Serial.print("<<--OVEN--> " );
  Serial.println(IamInOven);
  interrupts();
}


