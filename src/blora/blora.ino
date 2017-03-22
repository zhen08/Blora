#include "lora.h"
#include "ble.h"

// Singleton instance of the Lora driver
Lora lora(0);

// Singleton instance of the Ble driver
Ble ble;

int led = 13;

void setup()
{
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  //while (!Serial) ; // Wait for serial port to be available
  delay(1000);
  Serial.println("Starting");
  ble.init();
  lora.init();
}

uint8_t buf[255];
uint8_t len;
uint8_t i;

void loop()
{
  digitalWrite(led, HIGH);
  lora.poll();
  if (lora.dataAvailable())
  {
    lora.getData(buf, &len);
    ble.sendData(buf, &len);
  }
  digitalWrite(led, LOW);

  ble.poll();
  if (ble.dataAvailable())
  {
    ble.getData(buf, &len);
    for (i = 0; i < len; i++)
    {
      Serial.print((char)buf[i]);
    }
    if ((buf[0] == 'A') && (len > 1))
    {
      lora.setThisAddress(buf[1]);
    }
    else
    {
      lora.sendtoWait(buf, len, buf[0]);
    }
  }
}
