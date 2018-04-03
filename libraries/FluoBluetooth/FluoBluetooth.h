/**
  Sends data from the Arduino processor to users connected to the bluetooth module.
*/

#ifndef IOTFLUOROMETER_FLUOBLUETOOTH_H_
#define IOTFLUOROMETER_FLUOBLUETOOTH_H_

#include <Arduino.h>
//#include <SPI.h>
#include <BLEPeripheral.h>

#define BLE_REQ     5
#define BLE_RDY     4
#define BLE_RST     1

class FluoBluetooth {
  private:
	BLEPeripheral ble_peripheral;
	BLEService fluo_service;
	BLECharCharacteristic char_characteristic;
	BLEIntCharacteristic int_characteristic;
	BLEDoubleCharacteristic double_characteristic;
	BLELongCharacteristic long_characteristic;
	uint8_t type;
  String d_name;
  public:
  	FluoBluetooth(uint8_t u_type, String u_name) : ble_peripheral(BLE_REQ, BLE_RDY, BLE_RST), fluo_service("BBB0"),
  	 char_characteristic("BBB1", BLERead | BLENotify),
  	 int_characteristic("BBB2", BLERead | BLENotify), double_characteristic("BBB3", BLERead | BLENotify),
  	 long_characteristic("BBB4", BLERead | BLENotify){  //types: 0=char, 1=int, 2=double, 3=long
  	  type = u_type;
      d_name = u_name;
  	 }
     void init();
     void poll();
    template <typename T>
    void sendData(T);
    void changeType(uint8_t);

};

#endif // IOTFLUOROMETER_FLUOBLUETOOTH_H_
