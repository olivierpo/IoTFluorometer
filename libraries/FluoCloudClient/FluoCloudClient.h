/**
	Client to send data to Google Sheets using the Google sheets API and a
	Temboo Choreo.
**/

#ifndef IOTFLUOROMETER_FLUOCLOUDCLIENT_H_
#define IOTFLUOROMETER_FLUOCLOUDCLIENT_H_

#include <Arduino.h>

class FluoCloudClient {
  private:
    String index;
    String path_to_json;
    
    String path_to_python;
    bool appendToSheets(String);
    void updateData(String);
    void updateIndex();
  public:
  String spreadsheet_key;
    FluoCloudClient(String p_t_p, String p_t_j, String s_k);
    void sendToCloudFromStorage(String);
    void sendToCloudDirect(String);

    void setPathToPython(String);
    void setPathToJson(String);
    void setSpreadKey(String);
};

#endif // IOTFLUOROMETER_FLUOCLOUDCLIENT_H_

