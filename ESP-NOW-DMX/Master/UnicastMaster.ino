#include "MacList.h"

uint8_t unicastData[MAX_UNICAST_FRAME_SIZE];

void setupUnicast() {
  // TODO is this better working with uint8 ?!
  for (uint8_t i=0; i <= MAX_UNICAST_FRAME_SIZE; i++) {
    unicastData[i] = i;
  }
  WiFi.mode(WIFI_STA);
  if (VERBOSE) Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

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
}

// TODO what are you doing?! Cloning MAC for the setup function?
void copyArray(uint8_t array[6], uint8_t copy[6]) {
  for (int i=0; i <= 6; i++) {
    array[i] = copy[i];
  }
}

// send meta Data to Slave with BroadcastID or Unicast and Offset
void metaInformationToSlaves(const uint8_t *peer_addr, struct_advanced_meta metaData) {
  metaData.metaCode = 253;
  if(VERBOSE) {
    Serial.print("[Info] Send DMX Information ");
    Serial.print((int) sizeof(metaData));
    Serial.println(" (B)");
  }
  esp_err_t unicastResult = sendUnicastReliable(peer_addr, metaData);
                                        
  if(DEBUG) espNowStatus(unicastResult);
}

esp_err_t sendUnicastReliable(const uint8_t *peer_addr, struct_advanced_meta data) {
  esp_err_t unicastResult;
  while(!success) {
    Serial.println("reliable sending...");
    unicastResult = esp_now_send(peer_addr, 
                                          (uint8_t *) &data,
                                          sizeof(data));
    delay(WAIT_AFTER_SEND + WAIT_AFTER_REP_SEND + 5000);
  }
  success = 0;
  return unicastResult;
}

// send data
void sendDataEspUnicast(uint8_t repetition) {
  unicastData[1] = repetition;

  if(VERBOSE) Serial.println("[Info] Init DMX Unicasting");

  // Starting with 1 because zero is the BROADCAST ADDRESS
  for (int i = 1; i <= SLAVE_COUNT; i++) {
    if(VERBOSE) { 
      Serial.print("Unicast: "); Serial.println(i);
      for (int j=0; j < 6; j++) {
        Serial.print(SLAVE_MAC_ARRAY[i][j]);Serial.print(":");
      }
      Serial.println("");
    }

    setTimestamp();
    esp_err_t unicastResult = esp_now_send(SLAVE_MAC_ARRAY[i],
                                          (uint8_t *) &unicastData, // ??
                                          UNICAST_FRAME_SIZE);
    if(DEBUG) espNowStatus(unicastResult);
    delay(WAIT_AFTER_SEND); // No delay crashs the system
  }
}

/* select each node and send an request unicast */
void collectData(struct_advanced_meta metaData) {

  Serial.println(SLAVE_COUNT);
  
  for (int i = 1; i < SLAVE_COUNT+1; i++) {
    if(VERBOSE) { 
      Serial.print("Collect Data from fixture "); Serial.println(i);
      for (int j=0; j < 6; j++) {
        Serial.print(SLAVE_MAC_ARRAY[i][j], HEX);Serial.print(":");
      }
      Serial.println("");
    }

    metaData.metaCode = 254;
    esp_err_t unicastResult = esp_now_send(SLAVE_MAC_ARRAY[i],
                                          (uint8_t *) &metaData,
                                          sizeof(metaData));
    if(DEBUG) espNowStatus(unicastResult);
    delay(WAIT_AFTER_REP_SEND); // No delay crashs the system
    if (unicastResult != ESP_OK) {
      i--;
      Serial.println("Retry Unicast");
    }
  }
}