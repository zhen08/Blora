#ifndef Lora_h
#define Lora_h
#include <RH_RF95.h>
#include <RHMesh.h>

class Lora
{
public:
  Lora(uint8_t thisAddress = 0);
  bool init();
  void poll();
  bool sendtoWait(uint8_t *data, uint8_t len, uint8_t dest);
  void setThisAddress(uint8_t thisAddress);
  bool dataAvailable();
  bool getData(uint8_t *data, uint8_t *len);

private:
  RH_RF95 driver;
  RHMesh manager;
};

#endif //Lora_h
