/*=========================================================================
                         Test functions()
    -----------------------------------------------------------------------*/
void DOAtests(){
   while(!Serial);
   CreateFileList();
//SendOnline();
StartMesurment();
SendFile(g_FileName, 1,65535);
g_task = 0;

}//end of DOAtests()
