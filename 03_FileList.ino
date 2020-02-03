void CreateFileList() { //Function to create list of files
SdCardErrorsCheck();
 
 uint32_t startTime = millis();          // variables for time in ms
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
  ListOfFiles << "ClipID_Task;FileName;Counter;FileSize(KB);FileSize(byte);NumberOfLines;FileType;DirIndex" << endl;

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


if (NameOfFile[11] == 'd'){ //if DAT file then
    FileType = 1;
    numberOfLines = 0;
    OneFile.seekEnd(-sizeof(Payload)); //TAKE care that offset bigger then average line size in bytes!
    OneFile.read((uint8_t *)&Payload, sizeof(Payload)); // get from file blocks into Payload ( sizeof(Payload) )
    numberOfLines = Payload.Cell_3;
} //enf if DAT file then


      ii++;
     //cout << ClipID_Task << ";" << "PingTimeStamp" << ";" << "UnixTime" << ";" << ii << ";" << NameOfFile10 << ";" << AllFiles.fileSize() << ";" << numberOfLines << ";" << AllFiles.dirIndex() << endl;
     ListOfFiles << ClipID_Task << ";" << NameOfFile10  << ";" << ii << ";" << AllFiles.fileSize()/1000<< ";" << AllFiles.fileSize()%1000 << ";" << numberOfLines<< ";" << FileType << ";" << AllFiles.dirIndex() << endl;

      } //end of IF for no hidden files or directory

    AllFiles.close();

  }
  ListOfFiles.close();

  Serial.print("file list done in "); Serial.print ((millis() - startTime) / 1000); Serial.println(" seconds.");
  g_fileCount = ii;

} //Enf of function
