/*=========================================================================
                        Converting String to array of 16 Bytes
    -----------------------------------------------------------------------*/

void ConvertStringToArray (char TextString[], uint16_t *g_SendMsg, uint16_t task)
{ //function for converting string to uint16_t[] array


uint32_t StartTime = micros();
char *pch;                                          // temporal linked char[]  for STRTOK function
char TextStringCopy[strlen(TextString)];            // here we find lenghts of input TextString
strcpy(TextStringCopy, TextString);                 // and creating TextStringCopy of TextString
    
            
              pch = strtok (TextStringCopy, ";");                 //first text value from string before separator ";"
              g_SendMsg[0] = atoi(pch);
              
              pch = strtok (NULL, ";"); 
              g_SendMsg[2] = (uint16_t) ((atol(pch) >> 16) & 0xFFFF) ; 
              g_SendMsg[1] = (uint16_t) ((atol(pch)) & 0xFFFF) ;
              
              
              for (uint8_t i = 3; i <= 15; i++ ) {                  //getting NEXT values from strig befor separator ";" and put it into uint32_t data types (g_SendMsg[i])
                  pch = strtok (NULL, ";");                                
                  g_SendMsg[i] = atoi(pch);  
               } //end FOR  


uint32_t FinishTime = micros();


} //end ConvertStringToArray
