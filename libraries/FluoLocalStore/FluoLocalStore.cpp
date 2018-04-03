#include <FileIO.h>
#include "FluoLocalStore.h"

void FluoLocalStore::saveToUSB(String data, String path) {
  File dataFile = FileSystem.open(path.c_str(), FILE_APPEND);
  //Serial.println(F("Opening file and inputting data..."));
  if (dataFile) {
    dataFile.print(data+"\n");
    dataFile.close();
    //Serial.println(F("Data input to text file complete!\n"));
  }
  else{
    Serial.println(F("Failed to open file for storage"));
  }
}

unsigned long FluoLocalStore::getFileSize(String path) {
  File dataFile = FileSystem.open(path.c_str(), FILE_READ);
  unsigned long file_size = dataFile.size();
  dataFile.close();
  return file_size;
}

void FluoLocalStore::deleteFile(String path) {
  FileSystem.remove(path.c_str());
}


