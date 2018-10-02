
#include "avdweb_AnalogReadFast.h"

const byte adcPin = A1; 
uint32_t StartTime;
uint32_t FinishTime;
 
void setup(void) 
{ Serial.begin(9600);
  while(!Serial);
  Serial.println( "\nanalogRead_10bit us1 analogRead_12bit us2 analogReadFast_10bit us3 analogReadFast_12bit us4");
  for(int i=0; i<10; i++) testAnalogRead();




  delay(3000);
  NVIC_SystemReset(); //reset CPU function
  //
}

void testAnalogRead()
{ 
  //delay(1000);
StartTime = micros();
  
//analogReadResolution(10); // restore default
//  int adc1 = analogRead(adcPin); // default resolution 10bit 425us on SAMD21 
//FinishTime = micros(); Serial.print (adc1); Serial.print ( "          analogRead(adcPin)10 in micros ");Serial.println ( FinishTime - StartTime);StartTime = micros();
//
//
//  analogReadResolution(10);
//  int adc3 = analogReadFast(adcPin); // 23us on SAMD21  
//FinishTime = micros(); Serial.print (adc3); Serial.print ( "          analogReadFast(adcPin)10 in micros ");Serial.println ( FinishTime - StartTime);StartTime = micros();

 analogReadResolution(12); 
  int adc2 = analogRead(adcPin); // 425us on SAMD21 
FinishTime = micros(); Serial.print (adc2); Serial.print ( "          analogRead(adcPin)12 in micros ");Serial.println ( FinishTime - StartTime);StartTime = micros();
 
  analogReadResolution(12);
  int adc4 = analogReadFast(adcPin); // 24us on SAMD21 
FinishTime = micros(); Serial.print (adc4); Serial.print ( "          analogReadFast(adcPin)12 in micros ");Serial.println ( FinishTime - StartTime);StartTime = micros();

 

}

void loop(void) 
{  
  
}



