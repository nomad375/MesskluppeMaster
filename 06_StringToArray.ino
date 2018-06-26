/*=========================================================================
                        Converting String to array of 32 Bytes
    -----------------------------------------------------------------------*/

void ConvertStringToArray (char str[], uint32_t *g_SendMsg)
{ //function for converting string to uint32_t[] array

  int len = strlen(str);                      // STRTOK modifing input char*, so we prevent modification of original string
  char copy[len];                             // here we find lenghts of input STR
  strcpy(copy, str);                          // and creating COPY of STR

  
  byte i = 0;                                         //i - counter for uint32_t array position for fist sensor. positions 0 and 1 reserved for time value

  char *pch;                                                   // temporal linked char[]  for STRTOK function
 
  pch = strtok (copy, ";");                                    //first text value from string before separator ";"
  g_SendMsg[0] = atol(pch);  
 
  for (uint8_t i = 1; i <8; i++ ) {            // 
    pch = strtok (NULL, ";");                                 //getting NEXT values from strig befor separator ";" and put it into uint16_t data types
    g_SendMsg[i] = atol(pch);                          //getting uint16_t inside Uint16FromString[i], where i>1 and < NumberOfSENSOR_COUNT
  } //end IF


 

  //Serial.print  (" STR: StringFromSD[] =  "); Serial.println (str);
  //Serial.print  ("COPY: StringFromSD[] =  "); Serial.println (copy);
  //printing of arrays for checking
  //Serial.println("Function test - DEC of MyTime: "); Serial.print(myTime, DEC); Serial.println(" ");
  //Serial.println("Function test - HEX of MyTime: "); Serial.print(myTime, HEX); Serial.println(" ");
  //Serial.print (str);
//Serial.print("Function test - array of uint32_t[]: "); for (int i = 0; i < 8; i++) { Serial.print(g_SendMsg[i], HEX); Serial.print("  ");  }  Serial.println();
//Serial.print("Function test - array of uint32_t[]: "); for (int i = 0; i < 8; i++) { Serial.print(g_SendMsg[i]); Serial.print("  ");  }  Serial.println();
 
  //   Serial.print("Function test - array of uint8_t[]:  "); for (int i = 0; i < Array8Dimention; i++) { if (Uint8FromString[i]<16) {Serial.print ("0");}  Serial.print(Uint8FromString[i], HEX); Serial.print(" "); }  Serial.println();


} //end ConverStringToUint16

