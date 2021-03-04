#include "MacList.h"

/* all broadcast spesific functions */

// Setup dmx broadcast messages
struct_dmx_message broadcastData1;
struct_dmx_message broadcastData2;
struct_dmx_message broadcastData3;
struct_dmx_message broadcastData4;
struct_dmx_message broadcastData5;
struct_dmx_message broadcastArray[5];

void setupBroadcast() {
  if(DEBUG) Serial.println("Setup Broadcast");
  // BROADCAST:
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData1.dmxFrame[i] = i; }
  broadcastData1.broadcastId = 1;
  broadcastArray[0] = broadcastData1;
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData2.dmxFrame[i] = i; }
  broadcastData2.broadcastId = 2;
  broadcastArray[1] = broadcastData2;
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData3.dmxFrame[i] = i; }
  broadcastData3.broadcastId = 3;
  broadcastArray[2] = broadcastData3;
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData4.dmxFrame[i] = i; }
  broadcastData4.broadcastId = 4;
  broadcastArray[3] = broadcastData4;

  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  InitESPNow();

  // adding broadcast "node" to the peerlist
  Serial.println("Adding Broadcast to peerlist:");
  addNodeToPeerlist(BROADCAST_MAC);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  if(DEBUG) esp_now_register_send_cb(onDataSent);

  // // esp_now_register_recv_cb(onDataRecvBroadcast);
}

// send meta Data to Slave with BroadcastID and Offset
void metaInformationToSlaves(const uint8_t *peer_addr, struct_advanced_meta metaData) {
  metaData.metaCode = 33;
  if(VERBOSE) {
    Serial.print("[Info] Send DMX Information ");
    Serial.print((int) sizeof(metaData));
    Serial.println(" (B)");
  }
  esp_err_t unicastResult = esp_now_send(peer_addr, 
                                        (uint8_t *) &metaData,
                                        sizeof(metaData));
                                        
  if(DEBUG) espNowStatus(unicastResult);
}

void createMetaPackage(){
  if (VERBOSE) Serial.println("createMetaPackage");
  advanced_meta.metaCode             = 1;
  advanced_meta.slave_offset         = 20;
  advanced_meta.slave_broadcastId    = 30;
  advanced_meta.verbose              = VERBOSE;
  advanced_meta.debug                = DEBUG;
  advanced_meta.timestamp            = TIMESTAMP;
  advanced_meta.airtime              = AIRTIME;
  advanced_meta.full_repetitions     = FULL_REPETITIONS;
  advanced_meta.master_channel       = MASTER_CHANNEL;
  advanced_meta.slave_channel        = SLAVE_CHANNEL;
  advanced_meta.dmx_broadcasting     = DMX_BROADCASTING;
  advanced_meta.channel_total        = CHANNEL_TOTAL;
  advanced_meta.broadcast_frame_size = BROADCAST_FRAME_SIZE;
  advanced_meta.unicast_frame_size   = UNICAST_FRAME_SIZE;
  advanced_meta.send_repitition      = SEND_REPITITION;
  advanced_meta.wait_after_send      = WAIT_AFTER_SEND;
  advanced_meta.wait_after_rep_send  = WAIT_AFTER_REP_SEND;
}

void sendDataEspBroadcast() {
  if(VERBOSE) Serial.println("[Info] Init DMX Broadcasting");

  for (int j = 0; j < CHANNEL_TOTAL; j+=BROADCAST_FRAME_SIZE) {
    int i = j/BROADCAST_FRAME_SIZE;
    if(DEBUG) { 
      Serial.print("[OK] DMX Broadcast "); 
      Serial.println(broadcastArray[i].broadcastId); 
    }
    // Measure airtime with timestamp
    esp_err_t broadcastResult = esp_now_send(BROADCAST_MAC,
                                            (uint8_t *) &broadcastArray[i].dmxFrame,
                                            BROADCAST_FRAME_SIZE);
                                            // // sizeof(broadcastArray[i].dmxFrame)); // == MAX_BROADCAST_FRAME_SIZE
    if(DEBUG) espNowStatus(broadcastResult);
    delay(WAIT_AFTER_SEND); // TODO: No delay crashs the system
  }
}