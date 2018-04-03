#include "FluoBluetooth.h"

void FluoBluetooth::init() {
  char device_name[d_name.length()+1];
  d_name.toCharArray(device_name, sizeof(device_name));

  BLEDescriptor fluo_descriptor = BLEDescriptor("2901", device_name);
  // set advertised local name and service UUID
  ble_peripheral.setLocalName(device_name);  // Advertised in scan data as part of GAP
  ble_peripheral.setDeviceName(device_name); // Advertised in generic access as part of GATT
  ble_peripheral.setAdvertisedServiceUuid(fluo_service.uuid());

  // add service and characteristics
  ble_peripheral.addAttribute(fluo_service);
  
  ble_peripheral.addAttribute(fluo_descriptor);
  switch(type){
    case 0: 
        ble_peripheral.addAttribute(char_characteristic);
        break;
    case 1: 
        ble_peripheral.addAttribute(int_characteristic);
        break;
    case 2: 
        ble_peripheral.addAttribute(double_characteristic);
        break;
    default: 
        ble_peripheral.addAttribute(long_characteristic);
  }

    // begin initialization
  ble_peripheral.begin();
}

template <typename T>
void FluoBluetooth::sendData(T data){
  switch(type){
    case 0: 
        char_characteristic.setValue(data);
        break;
    case 1: 
        int_characteristic.setValue(data);
        break;
    case 2: 
        double_characteristic.setValue(data);
        break;
    default: 
        long_characteristic.setValue(data);
        break;
  }
  ble_peripheral.poll();
}
void FluoBluetooth::poll(){
  ble_peripheral.poll();
}

void FluoBluetooth::changeType(uint8_t u_type){
  type = u_type;
}

template void FluoBluetooth::sendData<char>(char data);
template void FluoBluetooth::sendData<int>(int data);
template void FluoBluetooth::sendData<double>(double data);
template void FluoBluetooth::sendData<long>(long data);
