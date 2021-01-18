#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>

void setupSlaveUnicast() {
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecvUnicast);
}

// callback when data is recv from Master just printing incomming data
// TODO devide in Broadcast & Unicast
void OnDataRecvUnicast(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  if (AIRTIME) {
    Serial2.print("!");
  }
  // memcpy(&dmx_meta, incommingData, sizeof(dmx_meta));
  memcpy(&espUnicastData, incommingData, sizeof(espUnicastData));
  
  // DMX DATA PACKAGE HANDLING
  if (VERBOSE) {
    Serial.print("espUnicastData.payload[0] =");Serial.println(espUnicastData.payload[0]);
    Serial.print("espUnicastData.payload[1] =");Serial.println(espUnicastData.payload[1]);
  }

  // check unicast integrity
  // iterating through the payload
  bool signalBroken = false;
  for (int i=1; i < data_len; i++) { 
    // just select needed channel
    if ((i >= offset) && (i < offset + CHANNELS_NEEDED)) {
      Serial.print(i); 
      Serial.print(" = "); 
      Serial.println(espUnicastData.payload[i]); 
    }
    if (espUnicastData.payload[i] != i) {
      signalBroken = true;
    }
  }
  if (signalBroken) {
    if(DEBUG) {
      Serial.print("[ERROR] Incomming Data broken: "); 
      Serial.print(data_len);
      Serial.println(" B");
    }
  }
  else {
    if(DEBUG) { 
      Serial.print("[OK] Rcvd: "); 
      Serial.print(data_len);
      Serial.println(" B");

      // TODO: timestamp here...
    }
  }
  getTimestamp();
  setTimestamp();
}