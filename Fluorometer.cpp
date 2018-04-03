#include "FluoWrapper.h"
FluoWrapper wrapper;

void setup()
{
  wrapper.setSleepTime(30);
  wrapper.setCanSleep(true);
  wrapper.setDrawInterval(10);
  wrapper.setSendInterval(40);
  wrapper.setTimeBeforeSleep(5);
  wrapper.setCanStore(true);
  wrapper.setCanSend(true);
  wrapper.setSizeCapEnabled(true);
  wrapper.setSizeCap(1000);
  wrapper.setSendIntervalEnabled(true);
  wrapper.setPathToFile("/mnt/sdb1/testfolder/datalog");

  wrapper.setPathToJson("/mnt/sdb1/testfolder/YunToCloud-a9572c8560ed.json");
  wrapper.setPathToPython("/mnt/sdb1/testfolder/googleSpread.py");
  wrapper.setSpreadKey("1cgSD0VVSioqxxj_yS5CB060Dc6318NMllCBwDQ79DzI");
}

void loop()
{

  wrapper.manageButtons();
  //wrapper.collectAndStoreData();
  wrapper.displayTime();
 // wrapper.managePowerSave();
}

