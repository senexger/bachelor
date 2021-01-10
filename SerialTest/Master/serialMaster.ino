/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
*/
#include <HardwareSerial.h>

// #define RXD2 16
// #define TXD2 17

//   // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
//   Serial.begin(115200);
//   //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
//   Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
//   Serial.println("Serial Txd is on pin: "+String(TX));
//   Serial.println("Serial Rxd is on pin: "+String(RX));

#define TIMESTAMP 1

// timestamps
unsigned long timestamp;
unsigned long timediff;

// UART checking
HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

void setup() 
{
  Serial.begin(115200);  //open serial via USB to PC on default port
  hSerial.begin(115200); //open the other serial port
}

void loop() 
{  
  setTimestamp();
  Serial2.print("test");
  //check incoming on other serial from the other board
  if (hSerial.available()) {
    getTimestamp();
    delay(200);
  }
}

void setTimestamp() {
  timestamp = (unsigned long) (esp_timer_get_time() );
  return;
}
unsigned long getTimestamp() {
  timediff = (unsigned long) (esp_timer_get_time() ) - timestamp;
  if (TIMESTAMP) {
    // Serial.print("[T] "); 
    Serial.println(timediff);
  }
  return timediff;
}