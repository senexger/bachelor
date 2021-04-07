#include "MacList.h"

/* all broadcast spesific functions */

// Setup dmx broadcast messages
struct_broadcast_message broadcastData1;
struct_broadcast_message broadcastData2;
struct_broadcast_message broadcastData3;
struct_broadcast_message broadcastData4;
struct_broadcast_message broadcastData5;
struct_broadcast_message broadcastArray[5];

void setupBroadcast() {
  if(DEBUG) Serial.println("Setup Broadcast");
  // BROADCAST:
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData1.dmxFrame[i] = i; }
  broadcastData1.broadcastId = 1;
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData2.dmxFrame[i] = i; }
  broadcastData2.broadcastId = 2;
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData3.dmxFrame[i] = i; }
  broadcastData3.broadcastId = 3;
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData4.dmxFrame[i] = i; }
  broadcastData4.broadcastId = 4;
  for (int i=1; i < BROADCAST_FRAME_SIZE +1; i++) { broadcastData5.dmxFrame[i] = i; }
  broadcastData5.broadcastId = 5;
  broadcastArray[0] = broadcastData1;
  broadcastArray[1] = broadcastData2;
  broadcastArray[2] = broadcastData3;
  broadcastArray[3] = broadcastData4;
  broadcastArray[4] = broadcastData5;

  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  InitESPNow();

  if (VERBOSE) Serial.println("Adding Broadcast to peerlist");
  addNodeToPeerlist(SLAVE_MAC_ARRAY[0]);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  if(DEBUG) esp_now_register_send_cb(onDataSent);

  // // esp_now_register_recv_cb(onDataRecvBroadcast);
}

// send meta Data to Slave with BroadcastID and Offset
void metaInformationToSlaves(const uint8_t *peer_addr, struct_advanced_meta metaData) {
  metaData.metaCode = 253;
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
  advanced_meta.unicast_slave_count  = UNICAST_SLAVE_COUNT;
  advanced_meta.send_repitition      = SEND_REPITITION;
  advanced_meta.wait_after_send      = WAIT_AFTER_SEND;
  advanced_meta.wait_after_rep_send  = WAIT_AFTER_REP_SEND;
}

void sendDataEspBroadcast(uint8_t repition) {
  broadcastArray[0].dmxFrame[0] = repition;

  if(VERBOSE) Serial.println("[Info] ESP DATA Broadcasting");

  for (int i = 0; i*BROADCAST_FRAME_SIZE < CHANNEL_TOTAL ; i++) {
    if(VERBOSE) { 
      Serial.print("[Info] DATA Broadcast "); 
      Serial.print(broadcastArray[i].broadcastId); 
      Serial.print(" (");
      Serial.print((i+1)*BROADCAST_FRAME_SIZE);Serial.print("/");Serial.print(CHANNEL_TOTAL);
      Serial.println(")");
    }

    setTimestamp();
    esp_err_t broadcastResult = esp_now_send(SLAVE_MAC_ARRAY[0], // == BROADCAST_MAC
                                            (uint8_t *) &broadcastArray[i].dmxFrame,
                                            BROADCAST_FRAME_SIZE);
    if(DEBUG) espNowStatus(broadcastResult);
    delay(WAIT_AFTER_SEND); // No delay crashs the system
  }
}