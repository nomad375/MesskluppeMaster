/*=========================================================================
                         SdCardErrorsCheck()
    -----------------------------------------------------------------------*/

void SdCardErrorsCheck() {   /* SD card SETUP */


Serial.println("SdCardErrorsCheck()");



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

void DeleteFile(char *g_FileName) {
  SdCardErrorsCheck();
  if (sd.exists(g_FileName)) {
    //  Serial.print ("This file: " ); Serial.print(g_FileName);
    if (!sd.remove(g_FileName)) {
      // error("remove failed");
      exit;
    } //end if ERROR
  } //end IF exists

}// End of DeleteFile()

/*=========================================================================
                         Delete ALL Files
    -----------------------------------------------------------------------*/

//void DeleteAllFiles() {
//
//  char NameOfFile[15];
//  SdFile AllFilesInDir;
//  
//  SdCardErrorsCheck();
//  sd.vwd()->rewind();
//
//  // open next file in root directory.  The volume working directory, vwd, is root
//  while (AllFilesInDir.openNext(sd.vwd(), O_READ)) {
//      
//      if (!AllFiles.isSubDir() && !AllFiles.isHidden()) {//check if file hidden or dir
//            AllFilesInDir.getName(NameOfFile, 15); //last number is a filename size in bytes
//            if (!sd.remove(NameOfFile)) {
//              //error("remove failed");
//              Serial.println (NameOfFile);
//              exit;
//      } //end IF
//    Serial.print (NameOfFile); Serial.println (" deleted");
//    AllFilesInDir.close();
//
//  } //end While
//
//}// End of DeleteAllFiles()

