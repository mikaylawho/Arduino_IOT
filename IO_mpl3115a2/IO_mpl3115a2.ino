/**************************************************************************/
/*!
    @file     IO_mpl3115a2.ino
    @author   Mikel Hensley
    @license  

    Feeds data to Adafruit.IO via the MPL3115A2 Altimeter chip using HUZZAH ESP8266 wifi breakout board. 
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include "IO_config.h"

// Power by connecting Vin to 3-5V, GND to GND
// Uses I2C - connect SCL to the SCL pin, SDA to SDA pin
// See the Wire tutorial for pinouts for each Arduino
// http://arduino.cc/en/reference/wire
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();

// set up the feeds
AdafruitIO_Feed *temperature = io.feed("temperature");
AdafruitIO_Feed *altitude = io.feed("altitude");
AdafruitIO_Feed *pressure = io.feed("pressure");

void setup() {
  Serial.begin(115200);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
  }
  
  float pascals = baro.getPressure();
  // Our weather page presents pressure in Inches (Hg)
  // Use http://www.onlineconversion.com/pressure.htm for other units
  Serial.print(pascals/3377); Serial.println(" Inches (Hg)");

  pressure->save(pascals/3377);

  float altm = baro.getAltitude();
  Serial.print(altm); Serial.println(" meters");

  altitude->save(altm);

  float tempC = baro.getTemperature();
  Serial.print(tempC); Serial.println("*C");

  temperature->save(tempC);

  delay(60000);
}
