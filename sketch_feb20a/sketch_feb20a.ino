const int hallPin1 = 1;     // the number of the hall effect sensor pin...
//const int ledPin =  13;     // the number of the LED pin
// variables will change:
int hallState1 = 0;          // variable for reading the hall sensor status

void setup() {
  // initialize the LED pin as an output:
  //pinMode(ledPin, OUTPUT);      
  // initialize the hall effect sensor pin as an input:
  pinMode(0, INPUT_PULLUP); 
  pinMode(1, INPUT_PULLUP); 
  
  pinMode(5, OUTPUT); //SLEEP pin for INA and Hall Sensors
  digitalWrite(5, HIGH);

}

void loop(){
  // read the state of the hall effect sensor:
    digitalWrite(5, HIGH);//wake up INA125   
  
  Serial.print (digitalRead(0)); 
  Serial.print (" ");
    Serial.print (digitalRead(1));
      Serial.println (" ");
 //   Serial.println (RawTclip);
    
    

//  if (hallState == LOW) {     
//    // turn LED on:    
//    //digitalWrite(ledPin, HIGH);  
//    Serial.println ("HIGH");
//  } 
//  else {
//    // turn LED off:
//   //digitalWrite(ledPin, LOW); 
//       Serial.println ("LOW");
//  }
}
