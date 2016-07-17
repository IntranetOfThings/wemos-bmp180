#include <Homie.h>
#include <Adafruit_BMP085.h>

#define FW_NAME         "homie-bmp180"
#define FW_VERSION      "0.0.1"

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

#define PUB_INTERVAL    5   // seconds


// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c d ata - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here
Adafruit_BMP085 bmp;

HomieNode sensorNode("sensor", "sensor");

unsigned long lastPublish = 0;

void loopHandler() {
  if (millis() - lastPublish >= PUB_INTERVAL * 1000UL) {  
    float temp = bmp.readTemperature();
    float pressure = bmp.readPressure(); //unit is hpa

    if (Homie.setNodeProperty(sensorNode, "pressure", String(pressure/100), false) && Homie.setNodeProperty(sensorNode, "temperature", String(temp), false)) {
      lastPublish = millis();
    }
  }
}

void setup() {
  Homie.setFirmware(FW_NAME, FW_VERSION);

  Homie.registerNode(sensorNode);

  Homie.setLoopFunction(loopHandler);
  
  bmp.begin();
  Homie.setup();
}

void loop() {
  Homie.loop();
}
