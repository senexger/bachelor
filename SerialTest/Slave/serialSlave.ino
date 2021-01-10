/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
*/
#include <HardwareSerial.h>

HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

void setup() 
{
  Serial.begin(115200);//open serial via USB to PC on default port
  hSerial.begin(115200);//open the other serial port
}

void loop() 
{  
  if (Serial.available()) {       //check incoming on default serial (USB) from PC
    hSerial.write(Serial.read()); // read it from UBS and send it to hSerial 
  }

  // if (hSerial.available()) {      //check incoming on other serial from the other board
    // Serial.write(hSerial.read()); //read it from hSerial and send it to  UBS
  // }
  delay(200);
}