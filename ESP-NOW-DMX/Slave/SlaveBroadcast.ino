#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>

void applyMetaInformation(const uint8_t *incommingData, int data_len) {
  Serial.println("THIS IS META!!!");
  memcpy(&advanced_Meta, incommingData, sizeof(advanced_Meta));
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

  // PRINT SETTINGS
  printSettings();
  // broadcast specific information
  Serial.print("BroadcastID      : "); Serial.println(advanced_Meta.broadcastId);
  Serial.print("BroadcastOffset  : "); Serial.println(advanced_Meta.broadcastOffset);
  // generell information
  Serial.print("SlaveOffset      : "); Serial.println(advanced_Meta.slave_offset);
  Serial.print("SlaveBroadcastID : "); Serial.println(advanced_Meta.slave_broadcastId);
}

int checkPayload(const uint8_t *incommingData, int data_len ) {
  memcpy(&espBroadcastData, incommingData, sizeof(data_len));

  // check broadcast integrity
  // sub 1 becaus there is no broadcastID in payload
  // iterating through the payload
  bool signalHealthy = true;

  // if (espBroadcastData.broadcastID == broadcastId) {
    for (int i=1; i<data_len ; i++) { 
      // Print all data
      // if (VERBOSE) 
          // Serial.print(i); Serial.print(" -> ");Serial.println(incommingData[i]);
      // just select needed channel
      // if ((i >= offset) && (i < offset + CHANNELS_NEEDED)) {
      // }
      if (incommingData[i] != i) {
        Serial.print("[ERROR] "); Serial.print(incommingData[i]); Serial.print(" != "); Serial.println(i);
        signalHealthy = false;
      }
    }
  // }
  return signalHealthy;
}