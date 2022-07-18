String Eingabe;

HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins
 
unsigned long timestampS[1];
unsigned long timediffS[1];

void setup() { 
    hSerial.begin(115200);
    Serial.begin(115200);
    Serial.println("Receive");
}

void loop() { 
    Serial.println("neue Runde!");
    setTimestampS(0);
    Serial2.print("!");
    waitForSerial();
    Serial.println(getTimestampS(0));
    delay(500);
}

void waitForSerial() {
  while(true) {
    if(hSerial.available()) {
      Serial.write(hSerial.read());Serial.println("");
      break;
    }
  }
  Serial.println("End of Loop");
}

// Timestampings for transmission time measurments
void setTimestampS(int r) {
  timestampS[r] = (unsigned long) (esp_timer_get_time() );
  return;
}
unsigned long getTimestampS(int r) {
  timediffS[r] = (unsigned long) (esp_timer_get_time() ) - timestampS[r];
  Serial.println(timediffS[r]);
  return timediffS[r];
}