#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>

// callback when data is recv from Master just printing incoming data
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len) {
  getTimestamp();
  if(VERBOSE) {
    Serial.println("VERBOSE: OnDataRecv()");
    Serial.print("incomingData[0]: "); Serial.println(incomingData[0]);
    Serial.print("incomingData[1]: "); Serial.println(incomingData[1]);
    Serial.print("incomingData[2]: "); Serial.println(incomingData[2]);
  }
  // TODO ist das sinnvoll?!
  if (AIRTIME) {
    Serial2.print("!");
  }
  if (incomingData[0] == 253) { // == advanced_Meta.metaCode
    applyMetaInformation(incomingData, data_len);
    return;
  }
  if (incomingData[0] == 254) { // == advanced_Meta.
    sendResultsToMaster();
    return;
  }
  if (incomingData[0] == 255) { // == advanced_Meta.
    readPayload(incomingData, data_len);
    if (VERBOSE) Serial.println("Read Payload");
    for (int i = 0; i<SEND_REPITITION; i++)
      Serial.println(successRatioArray[i]);
  }
}

void readPayload(const uint8_t * incomingData, int data_len) {
  if (isPayloadCorrect(incomingData, data_len)) {
    successRatioArray[incomingData[0]] = 1;

    if(VERBOSE) { 
      Serial.print("[OK] Rcvd: "); 
      Serial.print(data_len);
      Serial.println(" B (not broken)");
    }
      correctCastCounter();
  }
  else {
    successRatioArray[incomingData[0]] = 2;
    if(DEBUG) Serial.print("[ERROR] Incoming Data broken: "); 
  }
  Serial.print("correctCastCount: "); Serial.println(correctCastCount);
  setTimestamp();
}

void correctCastCounter() {
  correctCastCount -= 1;
}

bool isPayloadCorrect(const uint8_t *incomingData, int data_len ) {
  memcpy(&espBroadcastData, incomingData, sizeof(data_len));

  // check broadcast integrity
  // sub 1 becaus there is no broadcastID in payload
  // iterating through the payload
  bool signalHealthy = true;

  // if (espBroadcastData.broadcastID == broadcastId) {
    for (int i=2; i<data_len ; i++) { 
      // Print all data
      // if (VERBOSE) 
          // Serial.print(i); Serial.print(" -> ");Serial.println(incomingData[i]);
      // just select needed channel
      // if ((i >= offset) && (i < offset + CHANNELS_NEEDED)) {
      // }
      if (incomingData[i] != i) {
        Serial.print("[ERROR] "); Serial.print(incomingData[i]); Serial.print(" != "); Serial.println(i);
        signalHealthy = false;
      }
    }
  // }
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