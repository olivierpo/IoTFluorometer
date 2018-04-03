/**
  Sends data from the Arduino processor to the openWRT processor for storage.
*/

#ifndef IOTFLUOROMETER_FLUOLOCALSTORE_H_
#define IOTFLUOROMETER_FLUOLOCALSTORE_H_

#include <Arduino.h>

class FluoLocalStore {
  public:
    void saveToUSB(String, String);
    unsigned long getFileSize(String);
    void deleteFile(String);
};

#endif // IOTFLUOROMETER_FLUOLOCALSTORE_H_

