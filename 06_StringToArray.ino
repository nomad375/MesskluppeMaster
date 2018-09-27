/*=========================================================================
                        Converting String to array of 32 Bytes
    -----------------------------------------------------------------------*/

void ConvertStringToArray (char TextString[], uint32_t *g_SendMsg)
{ //function for converting string to uint32_t[] array

//uint32_t StartTime = micros();

    
  char TextStringCopy[strlen(TextString)];            // here we find lenghts of input TextString
  strcpy(TextStringCopy, TextString);                 // and creating TextStringCopy of TextString
  char *pch;                                          // temporal linked char[]  for STRTOK function

  pch = strtok (TextStringCopy, ";");                 //first text value from string before separator ";"
  g_SendMsg[0] = atol(pch);

  for (uint8_t i = 1; i < 8; i++ ) {                  //getting NEXT values from strig befor separator ";" and put it into uint32_t data types (g_SendMsg[i])
    pch = strtok (NULL, ";");                                
    g_SendMsg[i] = atol(pch);                         
  } //end FOR

  //Serial.print (str);
 // Serial.print("Function test - array of uint32_t[]: "); for (int i = 0; i < 8; i++) { Serial.print(g_SendMsg[i], HEX); Serial.print("  ");  }  Serial.println();
 // Serial.print("Function test - array of uint32_t[]: "); for (int i = 0; i < 8; i++) { Serial.print(g_SendMsg[i]); Serial.print("  ");  }  Serial.println();

//uint32_t FinishTime = micros();

//Serial.println ( FinishTime - StartTime);

} //end ConvertStringToArray

