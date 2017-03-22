#include "ble.h"
#include "BluefruitConfig.h"

#define MINIMUM_FIRMWARE_VERSION "0.6.6"
#define MODE_LED_BEHAVIOUR "MODE"

char rx_buffer[BUFSIZE];
uint8_t rx_buffer_ptr;
bool rx_available;

Ble::Ble()
    : ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST)
{
}

bool Ble::init()
{
  Serial.print("BLE Buffer Size:");
  Serial.println(BUFSIZE);
  if (!ble.begin(VERBOSE_MODE))
  {
    Serial.println("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?");
    return false;
  }
  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println("Performing a factory reset: ");
  if (!ble.factoryReset())
  {
    Serial.println("Couldn't factory reset");
    return false;
  }
  /* Disable command echo from Bluefruit */
  ble.echo(false);

  /* Print Bluefruit information */
  ble.info();

  ble.verbose(false); // debug info is a little annoying after this point!

  /* Wait for connection */
  while (!ble.isConnected())
  {
    delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if (ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION))
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set module to DATA mode
  Serial.println(F("Switching to DATA mode!"));
  ble.setMode(BLUEFRUIT_MODE_DATA);

  rx_buffer_ptr = 0;
  rx_available = false;
  return true;
}

void Ble::poll()
{
  while (ble.available())
  {
    int c = ble.read();
    if (c == '.')
    {
      rx_available = true;
    }
    else
    {
      rx_buffer[rx_buffer_ptr] = c;
      rx_buffer_ptr++;
    }
  }
}

bool Ble::dataAvailable()
{
  return rx_available;
}

bool Ble::sendData(uint8_t *data, uint8_t *len)
{
  ble.print((char *)data);
  return true;
}

void Ble::getData(uint8_t *data, uint8_t *len)
{
  memcpy(data, rx_buffer, rx_buffer_ptr);
  *len = rx_buffer_ptr;
  rx_buffer_ptr = 0;
  rx_available = 0;
}
