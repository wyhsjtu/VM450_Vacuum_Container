/***************************************************************************
How to compile - VM450
1. Open: Tool/ManageLibrary or Ctrl+Shift+I
2. Search for "Adafruit_BMP280"
3. Install the library
4. Set COM number, UNO/..., then compile
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13 //SCL
#define BMP_MISO 12 //SDA 
#define BMP_MOSI 11 //CSB
#define BMP_CS 10 //SDO

//Adafruit_BMP280 bme; // I2C
Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
  
void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280 test"));
  
  if (!bme.begin()) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}
  
void loop() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure());
    Serial.println(" Pa");

   // Serial.print("Approx altitude = ");
   // Serial.print(bme.readAltitude(1013.25)); // this should be adjusted to your local forcase
   // Serial.println(" m");
    
    Serial.println();
    delay(2000);
}
