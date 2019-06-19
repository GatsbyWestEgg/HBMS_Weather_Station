/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
char ssid[] = "yourssid";   // your network SSID (name)
char pass[] = "yourpw";

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
WiFiClient  client;

unsigned long myChannelNumber = 798570;
const char * myWriteAPIKey = "J0EWNJDSAES2JTVK";

unsigned long delayTime;
float temp = 0.0;  // initialise temperature value
float airP = 0.0;    // initialise air pressure value
float rh = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 20000;
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak


}


void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  printValues();
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, airP);
  ThingSpeak.setField(3,rh);
  
  ThingSpeak.setStatus("Weather Data Working");
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(delayTime);
}


void printValues() {
  temp = bme.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");
  
  airP = bme.readPressure() / 100.0F;
  Serial.print("Pressure = ");
  Serial.print(airP);
  Serial.println(" hPa");

  rh = bme.readHumidity();
  Serial.print("Humidity = ");
  Serial.print(rh);
  Serial.println(" %");

  Serial.println();
}
