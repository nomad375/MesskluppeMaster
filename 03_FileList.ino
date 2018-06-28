void CreateFileList() { //Function to create list of files
SdCardErrorsCheck();
 
 uint32_t startTime = millis();          // variables for time in ms
  int i = 0;
  uint32_t numberOfLines, n;
  char line[64] = {}; // char[48] enough for 7 sensors to read in buffer
  uint32_t ii = 0;

  char NameOfFile[15], NameOfFile10[10]; //variables to read filename and to get only digital part of it.
  uint32_t ClipID_Task = 1030;


  SdFile AllFiles, OneFile;
  ofstream ListOfFiles;

  if (!sd.exists("files")) {
    sd.mkdir("files");
  }
  ListOfFiles.open("files/file.dir", O_CREAT | O_WRITE);
  ListOfFiles << "ClipID_Task;PingTimeStamp;UnixTime;FileName;FileSize;NumberOfLines;DirIndex" << endl;

  sd.vwd()->rewind();

  // open next file in root directory.  The volume working directory, vwd, is root
  while (AllFiles.openNext(sd.vwd(), O_READ)) {

    // Skip directories and hidden files.
    if (!AllFiles.isSubDir() && !AllFiles.isHidden()) {//check if file hidden or dir
      AllFiles.getName(NameOfFile, 15); //last number is a filename size in bytes
      strncpy(NameOfFile10, NameOfFile, 10); // Get FileName value for external use


      // open  file to find number of lines inside it

      SdFile OneFile(NameOfFile, O_READ);
      numberOfLines = 0;

      // read lines from the file. move cursor to pisition end-of-file minus offset
      OneFile.seekEnd(-64); //TAKE care that offset bigger then average line size in bytes!

      while ((n = OneFile.fgets(line, sizeof(line))) > 0) { //read all lines untull EOF /enf-of-file

        //Serial.print("Found line of ");Serial.print(n);Serial.print(" bytes: "); Serial.print(line);
        if (n > 0) { // udate value numberOfLines to value from not enpty line

          char *pch;                                                   // temporal linked char[]  for STRTOK (look for value in beatween of separators)
          pch = strtok (line, ";");                                    //first iteration
          numberOfLines = atol(pch);                                   //TEMPORAL value for numberOfLines

          for (uint8_t i = 1; i < 3; i++ ) {
            pch = strtok (NULL, ";");                                 //getting next values from strig befor separator ";"
            numberOfLines = atol(pch);                                //our target - value at position #3 (<3)
          } //end FOR

        }//end if
      
      } // end of read file line by line untill endOfFile-----------------------------------
      
      OneFile.close();
      ii++;
    // cout << ClipID_Task << ";" << "PingTimeStamp" << ";" << "UnixTime" << ";" << NameOfFile10 << ";" << AllFiles.fileSize() << ";" << numberOfLines << ";" << AllFiles.dirIndex() << endl;
      ListOfFiles << ClipID_Task << ";" << "PingTimeStamp" << ";" << "UnixTime" << ";" << ii << ";" << NameOfFile10 << ";" << AllFiles.fileSize() << ";" << numberOfLines << ";" << AllFiles.dirIndex() << endl;

      
    } //end of IF for no hidden files or directory

    AllFiles.close();

  }
  ListOfFiles.close();

  Serial.print("file list done in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds.");

} //Enf of function
