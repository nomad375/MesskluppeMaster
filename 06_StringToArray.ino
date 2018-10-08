/*=========================================================================
                        Converting String to array of 16 Bytes
    -----------------------------------------------------------------------*/

void ConvertStringToArray (char TextString[], uint16_t *g_SendMsg, uint16_t task)
{ //function for converting string to uint32_t[] array

// file structure:     //ID;Time;Line#;ms%100 ;FX ;FY ;FZ;AX ;AY;AZ;GX;GY;GZ;Tbr;Tcl;MX;MY;MZ;Vin

 /*
  Splitting int32_t variable to 2 uint16_t variables:
  intArray3[0] = (uint16_t) ((timeSeries[0] >> 16) & 0xFFFF) ;
  intArray3[1] = (uint16_t) ((timeSeries[0]) & 0xFFFF) ;
and back:
  timeSeries[6] = ( (intArray3[0] << 16) + (intArray3[1]) ); // Method 1
  timeSeries[7] = ( (int32_t) intArray3[0] ) << 16  | intArray3[1] ; // Method 2
  
  */


uint32_t StartTime = micros();
char *pch;                                          // temporal linked char[]  for STRTOK function
char TextStringCopy[strlen(TextString)];            // here we find lenghts of input TextString
strcpy(TextStringCopy, TextString);                 // and creating TextStringCopy of TextString
    
/*======== set how many values from file to send  =======*/
//  switch (task) {
//    case 40: //ClipID;Time;Line#;ms%100 ;FX ;FY ;FZ;AX ;AY;AZ;GX;GY;GZ;Tbr;Tcl
//                                       // temporal linked char[]  for STRTOK function
              
              pch = strtok (TextStringCopy, ";");                 //first text value from string before separator ";"
              g_SendMsg[0] = atoi(pch);
              
              pch = strtok (NULL, ";"); 
              g_SendMsg[2] = (uint16_t) ((atol(pch) >> 16) & 0xFFFF) ; /// hERE
              g_SendMsg[1] = (uint16_t) ((atol(pch)) & 0xFFFF) ;
              
              
              for (uint8_t i = 3; i <= 15; i++ ) {                  //getting NEXT values from strig befor separator ";" and put it into uint32_t data types (g_SendMsg[i])
                  pch = strtok (NULL, ";");                                
                  g_SendMsg[i] = atoi(pch);  
               } //end FOR  
//              PayloadLen = 32 ;      
//              break;

//    case 41: //  ClipID;Time;Line#;ms%100 ;FX ;FY ;FZ
//              pch = strtok (TextStringCopy, ";");                 //first text value from string before separator ";"
//              g_SendMsg[0] = atoi(pch);
//              
//              pch = strtok (NULL, ";"); 
//              g_SendMsg[1] = (uint16_t) ((atol(pch) >> 16) & 0xFFFF) ;
//              g_SendMsg[2] = (uint16_t) ((atol(pch)) & 0xFFFF) ;
//              
//              
//              for (uint8_t i = 3; i <= 7; i++ ) {                  //getting NEXT values from strig befor separator ";" and put it into uint32_t data types (g_SendMsg[i])
//                  pch = strtok (NULL, ";");                                
//                  g_SendMsg[i] = atoi(pch);  
//               } //end FOR    
//               PayloadLen = 8 ;      
//              break;
//  }   //end task


  //Serial.print (str);
 // Serial.print("Function test - array of uint32_t[]: "); for (int i = 0; i < 8; i++) { Serial.print(g_SendMsg[i], HEX); Serial.print("  ");  }  Serial.println();
 // Serial.print("Function test - array of uint32_t[]: "); for (int i = 0; i < 8; i++) { Serial.print(g_SendMsg[i]); Serial.print("  ");  }  Serial.println();

uint32_t FinishTime = micros();
//Serial.print ( "micros for line convert ");
//Serial.println ( FinishTime - StartTime);
//Serial.print("Function test - array of uint16_t[]: "); for (int i = 0; i < 16; i++) { Serial.print(g_SendMsg[i]); Serial.print("  ");  }  Serial.println();
//Serial.print("Function test - array of uint16_t[]: "); for (int i = 0; i < 16; i++) { Serial.print(g_SendMsg[i],HEX); Serial.print("  ");  }  Serial.println();


} //end ConvertStringToArray

