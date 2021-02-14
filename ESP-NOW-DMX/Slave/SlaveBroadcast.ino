#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>

void setupSlaveBroadcasting() {
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecvBroadcast);
}

// callback when data is recv from Master just printing incomming data
// TODO devide in Broadcast & Unicast
void OnDataRecvBroadcast(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  if(VERBOSE) Serial.println("OnDataRecvBroadcast()");
  // TODO ist das sinnvoll?!
  if (AIRTIME) {
    Serial2.print("!");
  }
  // memcpy(&dmx_meta, incommingData, sizeof(dmx_meta));
  memcpy(&espBroadcastData, incommingData, sizeof(espBroadcastData));
  
  // DMX META PACKAGE HANDLING
  if (espBroadcastData.broadcastID == 0) {
    // memcpy(&dmx_meta, incommingData, sizeof(dmx_meta));
    if(DEBUG) {
      Serial.print("DmxBit: "); Serial.println(espBroadcastData.broadcastID);
      Serial.print("ID    : "); Serial.println(espBroadcastData.payload[0]);
      Serial.print("Offset: "); Serial.println(espBroadcastData.payload[1]);
    }
    // set broadcastId and payload offset
    broadcastId = espBroadcastData.payload[0];
    offset = espBroadcastData.payload[1];
    // meta data received, so dont aks for them anymore in the loop
    isDmxMetaReceived = 1;
    Serial.println("ID IS ZERO!");
  }

  // ESP-BROADCAST DATA PACKAGE HANDLING
  else {
    if (VERBOSE) {
      Serial.print("espBroadcastData.broadcastID=");Serial.println(espBroadcastData.broadcastID);
      Serial.print("espBroadcastData.payload[0] =");Serial.println(espBroadcastData.payload[0]);
      Serial.print("espBroadcastData.payload[1] =");Serial.println(espBroadcastData.payload[1]);
    }

    // check broadcast integrity
    // sub 1 becaus there is no broadcastID in payload
    // iterating through the payload
    bool signalBroken = false;
    if (VERBOSE && espBroadcastData.broadcastID == broadcastId) {
      for (int i=1; i < data_len -1; i++) { 
        // just select needed channel
        if ((i >= offset) && (i < offset + CHANNELS_NEEDED)) {
          Serial.print(i); 
          Serial.print(" = "); 
          Serial.println(espBroadcastData.payload[i]); 
        }
        if (espBroadcastData.payload[i] != i) {
          signalBroken = true;
        }
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
    // TODO: sure?
    getTimestamp();
    setTimestamp();
  }
}

// TODO: Can this be removed?!
void sendESPCast(uint8_t mac_address[6]) {
  // TODO why should I not unicast?
  // * unicast: einfacher, aber wenn der Master geändert wird, müssen alle Slave benachrichtigt werden
  // * broadcast: wie können die Slaves erkennen der Broadcast keine wichtigen Daten enthält?!
  if(VERBOSE && DMX_BROADCASTING)  Serial.println("[Info] Begin BROADCAST");
  if(VERBOSE && !DMX_BROADCASTING) Serial.println("[Info] Begin UNICAST");
  esp_err_t broadcastResult = esp_now_send(mac_address,
                                          (uint8_t *) &slaveRequirements,
                                          sizeof(slaveRequirements));
  espNowStatus(broadcastResult);
}

// Callback when data is sent - Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.println("Last Packet Send Status");
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[ERROR] Delivery Fail");
  }
  else {
    Serial.println("[OK] Send Ack");
  }
}
