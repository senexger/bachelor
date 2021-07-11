#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>

// callback when data is recv from Master just printing incoming data
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len) {

  Serial2.print("!");

  getTimestamp();
  if(VERBOSE) {
    Serial.print("Receive: "); Serial.print(data_len); Serial.println(" (B)");
    printArray(incomingData, data_len);
  }

  if (incomingData[0] == 253) { // meta Daten
    applyMetaInformation(incomingData, data_len);
    sendAck();
    return;
  }
  if (incomingData[0] == 254) { // Results zurück senden bitte
    sendResultsToMaster();
    return;
  }
  if (incomingData[0] == 255) { // DMX Daten!
    readPayload(incomingData, data_len);
    if (VERBOSE) {
      Serial.println("Read Payload");
      for (int i = 0; i < SEND_REPITITION; i++) {
        Serial.print(successRatioArray[i]);
      }
    }
    Serial.println();
  }
}

void readPayload(const uint8_t * incomingData, int data_len) {
  if (isPayloadCorrect(incomingData, data_len)) {
    // hier steht in der ersten Zeile hardcodet die rep number
    if(incomingData[2] == 1) successRatioArray[incomingData[1]] += 1; 
    if(incomingData[2] == 2) successRatioArray[incomingData[1]] += 2; 
    if(incomingData[2] == 3) successRatioArray[incomingData[1]] += 4; 

    if(VERBOSE) { 
      Serial.print("[OK] Rcvd: "); 
      Serial.print(data_len);
      Serial.println(" B (not broken)");
    }
      correctCastCounter();
  }
  // TODO REMOVE
  else {
    successRatioArray[incomingData[0]] = 9;
    if(DEBUG) Serial.print("[ERROR] Incoming Data broken: "); 
  }
  Serial.print("correctCastCount: "); Serial.println(correctCastCount);
  setTimestamp();
}

void correctCastCounter() {
  correctCastCount -= 1;
}

// TODO REMOVE, not needed!
bool isPayloadCorrect(const uint8_t *incomingData, int data_len ) {
  memcpy(&espBroadcastData, incomingData, sizeof(data_len));

  bool signalHealthy = true;

  for (int i=3; i<data_len ; i++) { 
    if (incomingData[i] != i) {
      Serial.print("[ERROR] "); Serial.print(incomingData[i]); Serial.print(" != "); Serial.println(i);
      signalHealthy = false;
    }
  }
  return signalHealthy;
}

void applyMetaInformation(const uint8_t *incomingData, int data_len) {
  Serial.println("THIS IS META!!!");
  memcpy(&advanced_Meta, incomingData, sizeof(advanced_Meta));
  // APPLY SETTINGS
  VERBOSE              = advanced_Meta.verbose;
  DEBUG                = advanced_Meta.debug;
  TIMESTAMP            = advanced_Meta.timestamp;
  AIRTIME              = advanced_Meta.airtime;
  FULL_REPETITIONS     = advanced_Meta.full_repetitions;
  MASTER_CHANNEL       = advanced_Meta.master_channel;
  SLAVE_CHANNEL        = advanced_Meta.slave_channel;
  DMX_BROADCASTING     = advanced_Meta.dmx_broadcasting;
  CHANNEL_TOTAL        = advanced_Meta.channel_total;
  BROADCAST_FRAME_SIZE = advanced_Meta.broadcast_frame_size;
  UNICAST_FRAME_SIZE   = advanced_Meta.unicast_frame_size;
  RAPID_REPITITION     = advanced_Meta.rapid_repitition;
  SEND_REPITITION      = advanced_Meta.send_repitition;
  WAIT_AFTER_SEND      = advanced_Meta.wait_after_send;
  WAIT_AFTER_REP_SEND  = advanced_Meta.wait_after_rep_send;
  // broadcast settings
  broadcastId          = advanced_Meta.broadcastId;
  offset               = advanced_Meta.broadcastOffset;
  slave_offset         = advanced_Meta.slave_offset;
  slave_broadcastId    = advanced_Meta.slave_broadcastId;

  correctCastCount = SEND_REPITITION;
  Serial.print("correctCastCount"); Serial.println(correctCastCount);

  // PRINT SETTINGS
  printSettings();
  // broadcast specific information
  Serial.print("BroadcastID      : "); Serial.println(advanced_Meta.broadcastId);
  Serial.print("BroadcastOffset  : "); Serial.println(advanced_Meta.broadcastOffset);
  // generell information
  Serial.print("SlaveOffset      : "); Serial.println(advanced_Meta.slave_offset);
  Serial.print("SlaveBroadcastID : "); Serial.println(advanced_Meta.slave_broadcastId);
}

void sendAck() {
  Serial.println("Send Ack!");
  uint8_t ack = 1;
  esp_err_t ackResult = esp_now_send(MASTER_MAC, (uint8_t *) &ack, 1); //TODO MASTER_MAC
  espNowStatus(ackResult);
}