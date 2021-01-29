#include <HardwareSerial.h>
#include <WiFi.h>
#include "ArduinoJson.h"

void setup() {
  Serial.begin(115200); 

  // important?
  while(!Serial) {
  }
}

int status = 0;

void loop() {

  int size_ = 0;
  String payload;
  
  while ( !Serial.available()  ){
    // Serial.println("here");
  }
  
  if ( Serial.available() )
    payload = Serial.readStringUntil( '\n' );
  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println(error.c_str()); 
    return;
  }

  if (doc["mode"] == "broadcast") {
     Serial.println("{\"Success\":\"True\"}");
     status = 1;
  }
  else {
      Serial.println("{\"Success\":\"False\"}");
      status = -1;
   }
  delay(20);

}