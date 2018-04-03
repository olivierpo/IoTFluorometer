/**
	Wrapper class
**/

#ifndef IOTFLUOROMETER_FLUOWRAPPER_H_
#define IOTFLUOROMETER_FLUOWRAPPER_H_

#include <Arduino.h>
#include <FluoKeypad.h>
#include <FluoCloudClient.h>
#include <FluoOLED.h>


class FluoWrapper {
  private:
    //Amount of time the Arduino will sleep(in ms); must be 15, 30, 60, 120, 250, 500, 1000, 2000 or 4000
    uint16_t sleep_time;

    //Enable or disable sleeping
    bool can_sleep;

    //Amount of seconds of inaction before sleeping Arduino
    uint8_t time_before_sleep;

    //Enable or disable local storage of data
    bool can_store;

    //Enable or disable sending data to cloud
    bool can_send;

    //Enable or disable size cap of local storage
    bool size_cap_enabled;

    //Size cap of local storage file before sending/dumping(in Bytes)
    unsigned long size_cap;

    //Enable or disable time interval for sending/dumping data in local storage
    bool send_interval_enabled;

    //Amount of seconds between drawing and locally storing data
    uint16_t draw_interval;

    //Amount of seconds between sending locally stored data to cloud
    uint16_t send_interval;

    //Path to the local storage file
    String path_to_file;

    //FluoCloudClient class object for cloud storage
    FluoCloudClient cloud_client;

    //FluoOLED class object for output messages
    FluoOLED *oled;

    //FluoKeypad class object for input
    FluoKeypad *pad;

    uint16_t prev_print_time;
    double last_action_time;
    double time_pro_sleep;
    bool wokeup;
    int prev_multiple;
    double slept_time_millis;
    double lost_time;
    bool has_init;
    bool input_mode;

    void enterInputMode(){input_mode = true;}
    void exitInputMode(){input_mode = false;}
    uint16_t calibrateTime(double);
    void markAction();
    void init(uint16_t sleep_t, bool can_sl, uint16_t draw_int, uint16_t send_int, uint8_t time_b_s,
              bool can_se, bool can_st, bool size_c_e, unsigned long size_c, bool send_int_e, String path_to_f);
    void initP2();
  public:
    FluoWrapper() : cloud_client("", "", ""){
      init(120, false, 5, 10, 5, true, true, true, 1000, true, "/mnt/sdb1/testfolder/datalog.txt");
    }

    /**
       @param sleep_t
       Time in seconds to sleep the arduino

       @param can_s
       Allow arduino to sleep or not

       @param draw_int
       Interval between draws/local stores

       @param send_int
       Interval between sending local stored data to cloud

       @param time_b_s
       Inaction period before Arduino sleeps
    */
    FluoWrapper(uint16_t sleep_t, bool can_sl, uint16_t draw_int, uint16_t send_int, uint8_t time_b_s,
                bool can_se, bool can_st, bool size_c_e, unsigned long size_c, bool send_int_e, String path_to_f): cloud_client("", "", "")
    {
      init(sleep_t, can_sl, draw_int, send_int, time_b_s, can_se, can_st, size_c_e, size_c, send_int_e, path_to_f);
    }

    ~FluoWrapper(){
      delete pad;
      delete oled;
    }

    /**
       Draw data from sensor and store it locally and/or take local data and send it to cloud
       depending on draw_interval and send_interval variables.
    */
    void collectAndStoreData();

    /**
       Turn on power save mode depending on the time_before_sleep variable. (time spent idle will trigger sleep)
    */
    void managePowerSave();
    /**
       Check if a button was pushed, then trigger the response.
    */
    void manageButtons();

    /**
       Setter functions for the class variables.
    */
    void setSleepTime(uint16_t sleep_t) {
      sleep_time = sleep_t;
    }
    void setCanSleep(bool can_sl) {
      can_sleep = can_sl;
    }
    void setDrawInterval(uint16_t draw_int) {
      draw_interval = draw_int;
    }
    void setSendInterval(uint16_t send_int) {
      send_interval = send_int;
    }
    void setTimeBeforeSleep(uint8_t time_b_s) {
      time_before_sleep = time_b_s;
    }
    void setCanStore(bool can_st) {
      can_store = can_st;
    }
    void setCanSend(bool can_se) {
      can_send = can_se;
    }
    void setSizeCapEnabled(bool size_c_e) {
      size_cap_enabled = size_c_e;
    }
    void setSizeCap(unsigned long size_c) {
      size_cap = size_c;
    }
    void setSendIntervalEnabled(bool send_int_e) {
      send_interval_enabled = send_int_e;
    }
    void setPathToFile(String path_to_f) {
      path_to_file = path_to_f;
    }

    /**
       Necessary information for Cloud client to function properly.
    */
    void setPathToPython(String p_t_p) {
      cloud_client.setPathToPython(p_t_p);
    }
    void setPathToJson(String p_t_j) {
      cloud_client.setPathToJson(p_t_j);
    }
    void setSpreadKey(String s_k) {
      cloud_client.setSpreadKey(s_k);
    }

    /**
       Display elapsed time on lcd display.
    */
    void displayTime();
    void freeRAM();
};

#endif // IOTFLUOROMETER_FLUOWRAPPER_H_

