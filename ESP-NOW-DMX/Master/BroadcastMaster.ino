/* all broadcast spesific functions */

// Setup dmx broadcast messages
struct_dmx_message broadcastData1;
struct_dmx_message broadcastData2;
struct_dmx_message broadcastData3;
struct_dmx_message broadcastData4;
struct_dmx_message broadcastData5;
struct_dmx_message broadcastArray[5];

void setupBroadcast() {
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

  InitESPTimer();

  esp_now_register_recv_cb(onDataRecvBroadcast);
}

// void loopBroadcast() {
//   return;
// }

// send meta Data to Slave with BroadcastID and Offset
void sendUnicastBackToSlave(const uint8_t *peer_addr, struct_dmx_meta metaData) {
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

// TODO: depricated - now addNodeToPeer in utils.ino
// void selectNodeForUnicast(const uint8_t *mac_addr) {
//   // add peer to send the slave information
//   bool exists = esp_now_is_peer_exist(mac_addr);
//   if (!exists) {
//     memcpy(peer_info.peer_addr, mac_addr, 6);
//     esp_err_t status = esp_now_add_peer(&peer_info);
//     if (ESP_OK != status && DEBUG) {
//       Serial.println("[ERROR] Could not add peer"); }
//     else { 
//       if(DEBUG) Serial.println("[OK] Slave-peer added"); }
//   }
//   else {
//     if(DEBUG) Serial.println("[Warning] peer still exists");
//   }
// }

void sendDmxBroadcast() {
  if(VERBOSE) Serial.println("[Info] Init DMX Broadcasting");

  for (int j = 0; j < CHANNEL_TOTAL; j+=BROADCAST_FRAME_SIZE) {
    int i = j/BROADCAST_FRAME_SIZE;
    if(DEBUG) { 
      Serial.print("[OK] DMX Broadcast "); 
      Serial.println(broadcastArray[i].broadcastId); }
    // Measure airtime with timestamp
    esp_err_t broadcastResult = esp_now_send(BROADCAST_MAC,
                                            (uint8_t *) &broadcastArray[i],
                                            sizeof(broadcastArray[i]));
    if(DEBUG) espNowStatus(broadcastResult);
    delay(WAIT_AFTER_SEND); // No delay crashs the system
  }
}

// Split function to BroadcastMAster and UnicastMaster
// callback when data is recv from Slave
void onDataRecvBroadcast(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  memcpy(&slave_information, incommingData, sizeof(slave_information));
  // Print information of the incomming package
  if(DEBUG) { 
    Serial.print("[OK] received from "); 
    // print mac
    for (int i = 0; i < 6; i++)
    {
      Serial.printf("%02X", mac_addr[i]);
      if (i < 5)Serial.print(":");
    }
    Serial.print(" ("); 
    Serial.print(slave_information.channelCount); 
    Serial.println(" Channel)");
  }

  // add node to the peerlist, for transmitting a single unicast package
  addNodeToPeerlist(mac_addr);

  // TODO: Get values from MACList!!!
  dmx_meta.broadcastIdZero = 0;   // 0 -> boradcast contains just metainformation
  dmx_meta.broadcastId     = 1;   // where the slave has to read the information
  dmx_meta.broadcastOffset = 20;  // offset within the broadcast

  // send a massage back with the slave information
  if (DEBUG) {
    Serial.println("[info] Send Broadcast information back (as unicast)");
    Serial.print("broadcastIDZero: "); Serial.println(dmx_meta.broadcastIdZero);
    Serial.print("broadcastId:     "); Serial.println(dmx_meta.broadcastId);
    Serial.print("broadcastoffset: "); Serial.println(dmx_meta.broadcastOffset);
  }

  // send message
  sendUnicastBackToSlave(mac_addr, dmx_meta);

  // remove slave (needed?!) - not for DMX-Unicast!
  // esp_err_t status2 = esp_now_del_peer(&peer_info);
  // if (ESP_OK != status2) { Serial.println("Could not remove peer"); }
  // else { Serial.println("Slave-peer removed successfully"); }
}