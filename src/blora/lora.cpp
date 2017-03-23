#include "Lora.h"

#define RFM95_CS 10  // "B"
#define RFM95_RST 11 // "A"
#define RFM95_INT 6  // "D"

#define RF95_FREQ 915.0

uint8_t lora_rx_buf[RH_MESH_MAX_MESSAGE_LEN];
uint8_t lora_rx_len;
uint8_t lora_rx_available;

Lora::Lora(uint8_t thisAddress)
    : driver(RFM95_CS, RFM95_INT),
      manager(driver, thisAddress)
{
}

bool Lora::init()
{
  Serial.println("Initialising Lora Module");

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(200);
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!driver.init())
  {
    Serial.println("init failed");
    return false;
  }
  
  if (!driver.setModemConfig(RH_RF95::Bw125Cr48Sf4096))
  {
    Serial.println("setModemConfig failed");
    return false;
  }
  Serial.println("Set Modem Config to Bw125Cr48Sf4096");
  
  if (!driver.setFrequency(RF95_FREQ))
  {
    Serial.println("setFrequency failed");
    return false;
  }

  driver.setTxPower(23, false);
  Serial.println("Lora Module Initialised");
  Serial.print("RH_MESH_MAX_MESSAGE_LEN: ");
  Serial.println(RH_MESH_MAX_MESSAGE_LEN);
  lora_rx_available = false;

  manager.setArpTimeout(10000);
}

void Lora::poll()
{
  uint8_t len = sizeof(lora_rx_buf);
  uint8_t from;
  if (manager.recvfromAck(lora_rx_buf, &len, &from))
  {
    Serial.print("got request from : 0x");
    Serial.print(from, HEX);
    Serial.print(": ");
    Serial.println((char *)lora_rx_buf);
    Serial.print("RSSI: ");
    Serial.println(driver.lastRssi(), DEC);
    lora_rx_available = true;
  }
}

bool Lora::dataAvailable()
{
  return lora_rx_available;
}

bool Lora::getData(uint8_t *data, uint8_t *len)
{
  uint8_t actual_len = 0;
  while (lora_rx_buf[actual_len] != 0)
  {
    actual_len++;
  }
  actual_len++;
  memcpy(data, lora_rx_buf, actual_len);
  *len = actual_len;
  lora_rx_available = 0;
}

bool Lora::sendtoWait(uint8_t *data, uint8_t len, uint8_t dest)
{
  if (manager.sendtoWait(data, len, dest) != RH_ROUTER_ERROR_NONE)
  {
    Serial.print("Lora::sendtoWait Failed");
  return false;
  }
  return true;
}

void Lora::setThisAddress(uint8_t thisAddress)
{
  manager.setThisAddress(thisAddress);
  Serial.print("Local Address set to: ");
  Serial.println(thisAddress);
}
