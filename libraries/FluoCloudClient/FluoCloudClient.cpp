#include <FileIO.h>
#include <Bridge.h>
#include <Process.h>
#include "FluoCloudClient.h"

FluoCloudClient::FluoCloudClient(String p_t_p, String p_t_j, String s_k) {
  index = "1";
  path_to_python = p_t_p;
  path_to_json = p_t_j;
  spreadsheet_key = s_k;
}

bool FluoCloudClient::appendToSheets(String data) {
   // Serial.println(path_to_python);
  Bridge.put("JSON", path_to_json);
  Bridge.put("KEY", spreadsheet_key);
  updateData(data);
  Serial.println(F("Sending to Sheets..."));
  Process p;
  p.begin("python");
  p.addParameter(path_to_python);
  p.run();
  if (p.exitValue() == 0) {
    Serial.println(F("Send complete."));
    updateIndex();
    return true;
  }
  Serial.println(F("Send Failed."));
  return false;
}

void FluoCloudClient::sendToCloudFromStorage(String path) {
  Serial.println(F("Retrieving from Linux..."));
  File data_file1 = FileSystem.open(path.c_str(), FILE_READ);

  if (data_file1) {
    char c;
    String string_to_send = "";
    String full_read = "";
    boolean ended = true;
    int howmany = 0;

    int i = 0;

    while (data_file1.available()) {
      c = data_file1.read();
      if (c != '\n' && c != '\r') {
        full_read += String(c);
        ended = true;
      }
      else if (ended) {
        if (string_to_send.length() >= 15) {
          Serial.println(F("Too much data, truncating..."));
          appendToSheets(string_to_send);
          string_to_send = "";
        }
        string_to_send = string_to_send + full_read + "-";
        full_read = "";
        ended = false;
      }
    }
    data_file1.close();
    if (appendToSheets(string_to_send)) {
      Serial.println(F("-----------------------------------------------------------------------------------"));
      Serial.println(F("Data succesfully transmitted."));
      Serial.println(F("-----------------------------------------------------------------------------------\n"));
    }
  }
  else{
      Serial.print(F("could not open file for read."));
    }
}

void FluoCloudClient::updateData(String data) {
  Bridge.put("data", data);
  Bridge.put("index", index);
}

void FluoCloudClient::updateIndex() {
  char my_num[10];
  Bridge.get("index", my_num, 10);
  index = my_num;
}

void FluoCloudClient::setPathToJson(String p_t_j) {
  path_to_json = p_t_j;
}

void FluoCloudClient::setPathToPython(String p_t_p) {
  path_to_python = p_t_p;
}

void FluoCloudClient::setSpreadKey(String s_k) {
  spreadsheet_key = s_k;
}

void FluoCloudClient::sendToCloudDirect(String data) {
  appendToSheets(data);
}


