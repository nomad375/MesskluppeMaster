
#include "SAMD_AnalogCorrection.h"

// Load cells are linear. So once you have established two data pairs, you can interpolate the rest.
// Step 1: Upload this sketch to your arduino board

// You need two loads of well know weight. In this example A = 10 kg. B = 30 kg
// Put on load A
// read the analog value showing (this is analogvalA)
// put on load B
// read the analog value B

// Enter you own analog values here
float loadA = 0; // kg
int analogvalA = 400; // analog reading taken with load A on the load cell foe AREF external
//int analogvalA = 317; // analog reading taken with load A on the load cell

float loadB = 0.89; // kg
int analogvalB = 678; // analog reading taken with load B on the load cell

// Upload the sketch again, and confirm, that the kilo-reading from the serial output now is correct, using your known loads

long time_, time = 0; //
int timeBetweenReadings = 200; // We want a reading every 200 ms;
float load;
#include <ResponsiveAnalogRead.h>

ResponsiveAnalogRead analog1(A1, true);
//ResponsiveAnalogRead analog2(A2, true);
//ResponsiveAnalogRead analog3(A3, true);
//ResponsiveAnalogRead analog4(A4, true);


void setup() {
  Serial.begin(115200);
  analogReadCorrection(26, 2062);
  analogReadResolution(12); //12-bit resolution for analog inputs
  analogWriteResolution(10); //10-bit resolution for analog output A0
 // analogReference(AR_EXTERNAL);// external signal for analog reference

  analog1.setAnalogResolution(4096);
  analog1.setSnapMultiplier(0.01); // 0.01 by default
  analog1.enableEdgeSnap();
  //analog1.disableSleep();
  analog1.enableSleep();

  //analog2.setAnalogResolution(4096);
  //analog3.setAnalogResolution(4096);
  //analog4.setAnalogResolution(4096);



}

void loop() {

  //analogWrite(A0, 780); //2.5V for Aref input
  uint16_t i = 0;
  time = millis();
  time_ = micros();


  //Serial.print("analogValue in mV: ");Serial.print(analogValueAverage*2.4/4096*1000);
  //Serial.print("analogValue in mV: ");Serial.print(analogValueAverage*3.3/4096*1000);
//Serial.print(0);
//Serial.print(" ");
//Serial.print(2500);
//Serial.print(" ");
  // Serial.print("        analogValue Abs: ");Serial.print(AnalogValue());
  // Serial.print("        analogValue Avr: ");Serial.print(averageValue1);
  analog1.update(); Serial.print("        analogValue Raw: ");  Serial.print(analog1.getRawValue());
  //analog1.update(); Serial.print("        analogValue Flt in mV: ");  Serial.print(analog1.getValue());
  //analog2.update(); Serial.print("        analogValue Flt: ");  Serial.print(analog2.getValue());
  //analog3.update(); Serial.print("        analogValue Flt: ");  Serial.print(analog3.getValue());
 // analog4.update(); Serial.print("        analogValue Flt: ");  Serial.print(analog4.getValue());
 Serial.print("        loop time: "); Serial.println(micros() - time_);

  //  Is it time to print ?
//  if (millis() > time + timeBetweenReadings) {
//
//    analog1.update(); float load = analogToLoad(analog1.getValue());
//    Serial.print("       load: ");  Serial.println(load, 5);
//    time = millis();
//
//  } //end if Time

  Serial.println("");
  delayMicroseconds(2500);
}

float AnalogValue()
{

  float analogValue = analogRead(1);
  return analogValue;
} // end AnalogTemperature

float AverageValue()
{
  int MeasurementsToAverage = 10;
  float AverageValue = 0 ;
  for (int i = 0; i < MeasurementsToAverage; ++i)
  {
    AverageValue += analogRead(1);
    delayMicroseconds(100);
  }
  AverageValue /= MeasurementsToAverage;
  return AverageValue;
} // end AnalogTemperature



float analogToLoad(float analogval) {

  // using a custom map-function, because the standard arduino map function only uses int
  float load = mapfloat(analogval, analogvalA, analogvalB, loadA, loadB);
  return load;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
