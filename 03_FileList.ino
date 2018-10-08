void CreateFileList() { //Function to create list of files
SdCardErrorsCheck();
 
 uint32_t startTime = millis();          // variables for time in ms
  //int i = 0;
  uint32_t numberOfLines, n;
  char line[128] = {}; // char[48] enough for 7 sensors to read in buffer
  uint32_t ii = 0;

  char NameOfFile[15], NameOfFile10[10]; //variables to read filename and to get only digital part of it.
  uint32_t FileType; 
  uint32_t ClipID_Task = g_clipID * 1000 + 30; //case 30 "======= Get List============");
           
  SdFile AllFiles, OneFile;
  ofstream ListOfFiles;

  if (!sd.exists("files")) {
    sd.mkdir("files");
  }
  ListOfFiles.open("files/file.dir", O_CREAT | O_WRITE);
  ListOfFiles << "ClipID_Task;FileName;Counter;FileSize(KB);NumberOfLines;FileType;DirIndex" << endl;

  sd.vwd()->rewind();

  // open next file in root directory.  The volume working directory, vwd, is root
  while (AllFiles.openNext(sd.vwd(), O_READ)) {

    // Skip directories and hidden files.
    if (!AllFiles.isSubDir() && !AllFiles.isHidden()) {//check if file hidden or dir
      AllFiles.getName(NameOfFile, 256); //Save full filename into NameOfFile
      strncpy(NameOfFile10, NameOfFile, 10); // Get FileName10 value for external use

      SdFile OneFile(NameOfFile, O_READ);

      if (!OneFile.isOpen()) {
        SdCardErrorsCheck(); 
        exit(0);                        //exit from function if SD file cannot be opened
    }// endif

    
    if (NameOfFile[11] == 'c'){ //if CSV file then
      FileType = 02;
      // open  file to find number of lines inside it

      numberOfLines = 0;
      // read lines from the file. move cursor to position end-of-file minus offset
      OneFile.seekEnd(-128); //TAKE care that offset bigger then average line size in bytes!

      while ((n = OneFile.fgets(line, sizeof(line))) > 0) { //read all lines untull EOF /enf-of-file

        if (n > 0) { // udate value numberOfLines to value from not empty line

          char *pch;                                                   // temporal linked char[]  for STRTOK (look for value in beatween of separators)
          pch = strtok (line, ";");                                    //first iteration
          numberOfLines = atol(pch);                                   //TEMPORAL value for numberOfLines

          for (uint8_t i = 1; i < 4; i++ ) {
            pch = strtok (NULL, ";");                                 //getting next values from strig befor separator ";"
            numberOfLines = atol(pch);                                //our target - value at position #3 (<3)
          } //end FOR

        }//end if
      
      } // end of read file line by line untill endOfFile-----------------------------------
      
      OneFile.close();

} //enf if CSV file

if (NameOfFile[11] == 'd'){ //if CSV file then
    FileType = 01;
    numberOfLines = 0;
    OneFile.seekEnd(-sizeof(Payload)); //TAKE care that offset bigger then average line size in bytes!
    OneFile.read((uint8_t *)&Payload, sizeof(Payload)); // get from file blocks into Payload ( sizeof(Payload) )
    numberOfLines = Payload.Cell_3;
    PrintPayloadBytes();

} //enf if DAT file then


      ii++;
      //cout << ClipID_Task << ";" << "PingTimeStamp" << ";" << "UnixTime" << ";" << ii << ";" << NameOfFile10 << ";" << AllFiles.fileSize() << ";" << numberOfLines << ";" << AllFiles.dirIndex() << endl;
     // ListOfFiles << ClipID_Task << ";" << NameOfFile10  << ";" << ii << ";" << AllFiles.fileSize() << ";" << numberOfLines << ";" << AllFiles.dirIndex() << endl;
      ListOfFiles << ClipID_Task << ";" << NameOfFile10  << ";" << ii << ";" << AllFiles.fileSize()/1000<< ";" << AllFiles.fileSize()%1000 << ";" << numberOfLines<< ";" << FileType << ";" << AllFiles.dirIndex() << endl;

      
    } //end of IF for no hidden files or directory

    AllFiles.close();

  }
  ListOfFiles.close();

  Serial.print("file list done in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds.");

} //Enf of function
