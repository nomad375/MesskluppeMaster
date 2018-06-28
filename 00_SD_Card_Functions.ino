
/*=========================================================================
                         SdCardErrorsCheck()
    -----------------------------------------------------------------------*/

void SdCardErrorsCheck() {   /* SD card SETUP */

//Serial.println("SdCardErrorsCheck()");

  if (!sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED)) {    //Initialize the highest speed supported by the board (!sd.begin(SD_CHIP_SELECT, SD_SCK_MHZ(50)))
    
    // sd.initErrorHalt();                               //Print error and Halt
    sd.initErrorPrint();                                //Print error and try go on
  }
  if (sd.begin(SD_CHIP_SELECT, SPI_FULL_SPEED)) {    //Initialize the highest speed supported by the board (SD_CHIP_SELECT, SD_SCK_MHZ(50)))
    // check is card can be initialized e.g. card inside slot and can be read
    // Exit from loop if fine
    // or wait until card inserted in slot
    exit;
  }


} //end SdCardErrorsCheck()

/*=========================================================================
                         Delete File
    -----------------------------------------------------------------------*/

void DeleteFile(char *g_FileName){ 
SdCardErrorsCheck();
    if (sd.exists(g_FileName)){ 
        //  Serial.print ("This file: " ); Serial.print(g_FileName);
          if (!sd.remove(g_FileName)) {   
             // error("remove failed"); 
              exit;   
              } //end if ERROR
   } //end IF exists

}// End of DeleteFile()


