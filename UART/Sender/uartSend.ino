#include <HardwareSerial.h>
#include "ArduinoJson.h"

HardwareSerial &hSerial = Serial2;
unsigned long timestamp;
unsigned long timediff;

void setup() {
    hSerial.begin(115200);
    Serial.begin(115200);
    Serial.println("Sender");
}

void loop() {
    // if (hSerial.available()) {
    //   Serial2.write(hSerial.read());Serial2.println("");
    //   Serial.println("Send !");
    // }
  setTimestamp();
  delay(2);
  getTimestamp();
}

void setTimestamp() {
  timestamp = (unsigned long) (esp_timer_get_time() );
  return;
}
unsigned long getTimestamp() {
  timediff = (unsigned long) (esp_timer_get_time() ) - timestamp;
  Serial.println(timediff);
  return timediff;
}
