#include "MacList.h"

uint8_t unicastData[MAX_UNICAST_FRAME_SIZE];

void setupUnicast() {
  Serial.println("Setup Unicast");

  for (int i=0; i <= MAX_UNICAST_FRAME_SIZE; i++) {
    unicastData[i] = i;
  }

  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  InitESPNow();

  // adding peers to the peerlist - allowing to send esp-now
  Serial.println("Adding Slaves to peerlist:");
  addNodeToPeerlist(SLAVE_MAC_ARRAY[0]);
  addNodeToPeerlist(SLAVE_MAC_1);
  addNodeToPeerlist(SLAVE_MAC_2);
  addNodeToPeerlist(SLAVE_MAC_3);
  addNodeToPeerlist(SLAVE_MAC_4);
  addNodeToPeerlist(SLAVE_MAC_5);

  if(DEBUG) esp_now_register_send_cb(onDataSent);
  // TODO usage for acknoledgements?
  // esp_now_register_recv_cb(onDataRecvUnicast);
}

// TODO what are you doing?! Cloning MAC for the setup function?
void copyArray(uint8_t array[6], uint8_t copy[6]) {
  for (int i=0; i <= 6; i++) {
    array[i] = copy[i];
  }
}

// send data
void sendDataEspUnicast() {
  if(VERBOSE) Serial.println("[Info] Init DMX Unicasting");

  // Starting with 1 because zero is the BROADCAST ADDRESS
  for (int i = 1; i <= UNICAST_SLAVE_COUNT; i++) {
    if(VERBOSE) { 
      Serial.print("Unicast: "); Serial.println(i);
      for (int j=0; j < 6; j++) {
        Serial.print(SLAVE_MAC_ARRAY[i][j]);Serial.print(":");
      }
      Serial.println("");
    }

    esp_err_t unicastResult = esp_now_send(SLAVE_MAC_ARRAY[1], // ! HOTFIX
                                          (uint8_t *) &unicastData, // ??
                                          UNICAST_FRAME_SIZE);
    if(DEBUG) espNowStatus(unicastResult);
    delay(WAIT_AFTER_SEND); // No delay crashs the system
  }
}