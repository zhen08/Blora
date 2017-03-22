#ifndef Ble_h
#define Ble_h

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"

class Ble
{
public:
  Ble();
  bool init();
  void poll();
  bool dataAvailable();
  bool sendData(uint8_t *data, uint8_t *len);
  void getData(uint8_t *data, uint8_t *len);

private:
  Adafruit_BluefruitLE_SPI ble;
};
#endif //Ble_h
