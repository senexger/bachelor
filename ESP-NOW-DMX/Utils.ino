#include <esp_now.h>
#include <WiFi.h>
#include "MacList.h"

// depricated
// esp_now_peer_info_t slave;

// TODO: Understand how addNodeToPeerlist and addPeer are working

// peerlist information
esp_now_peer_info_t peer_info;

// peerlist information
void addNodeToPeerlist(const uint8_t *mac_addr) {
  // add peer to send the slave information
  bool exists = esp_now_is_peer_exist(mac_addr);
  if (!exists) {
    memcpy(peer_info.peer_addr, mac_addr, 6);
    esp_err_t status = esp_now_add_peer(&peer_info);
    if (ESP_OK != status && DEBUG) {
      Serial.println("[ERROR] Could not add peer"); }
    else { 
      if(DEBUG) Serial.println("[OK] Slave-peer added"); }
  }
  else {
    if(DEBUG) Serial.println("[Warning] peer still exists");
  }
}

// merge with addNodeToPeerlist
void addPeer (uint8_t mac_address[6]) {
  esp_now_peer_info_t peer_info;
  peer_info.channel = SLAVE_CHANNEL;
  memcpy(peer_info.peer_addr, mac_address, 6);
  peer_info.ifidx = ESP_IF_WIFI_STA;
  peer_info.encrypt = false;
  esp_err_t status = esp_now_add_peer(&peer_info);
  if (ESP_OK != status) { Serial.println("Could not add peer"); }
}

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow success");
  }
  else {
    Serial.println("ESPNow init failed");
    ESP.restart();
  }

  // add broadcast peer
  // peer_info.channel = MASTER_CHANNEL;
  // memcpy(peer_info.peer_addr, BROADCAST_MAC, 6);
  // peer_info.ifidx = ESP_IF_WIFI_STA;
  // peer_info.encrypt = false;
  // esp_err_t status = esp_now_add_peer(&peer_info);
  // if (ESP_OK != status)
  // {
  //   Serial.println("Could not add peer");
  // }
}

// +++ Timestamping +++

// using pointer would be more beautiful
unsigned long timestamp;
unsigned long timediff;

void setTimestamp() {
  timestamp = (unsigned long) (esp_timer_get_time() );
  return;
}
unsigned long getTimestamp() {
  timediff = (unsigned long) (esp_timer_get_time() ) - timestamp;
  Serial.print("[T] "); Serial.println(timediff);
  return timediff;
}

// Init ESP Timer with fallback
void InitESPTimer() {
  if (esp_timer_init() == ESP_OK) {
    Serial.println("[OK] ESPTimer Init Success");
  }
  else if (esp_timer_init() == ESP_ERR_NO_MEM) {
    Serial.println("[ERROR] ESPTimer allocation has failed");
  }
  else if (esp_timer_init() == ESP_ERR_INVALID_STATE) {
    Serial.println("[ERROR] ESPTimer already initialized");
  }
  else {
    Serial.println("[ERROR] ESPTimer Init Failed");
    ESP.restart();
  }
}

// Translating status of ESP-Now return
void espNowStatus(esp_err_t result) {
  // Print status of sended data
  if (result == ESP_OK) {
    Serial.println("[OK] Send");
    // Serial.print((int) sizeof(espBroadcastData));
    // Serial.println(" B");
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("[ERROR] ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("[ERROR] Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("[ERROR] Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("[ERROR] ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("[ERROR] Peer not found.");
  } else {
    Serial.println("[ERROR] Not sure what happened");
  }
}

void printSettings(){
  Serial.print("+++++++++++++");
  Serial.print(puropse);
  Serial.println("+++++++++++++");
  
  // Variables
  Serial.print("VERBOSE:                  ");Serial.println(VERBOSE);
  Serial.print("DEBUG:                    ");Serial.println(DEBUG);
  Serial.print("TIMESTAMP:                ");Serial.println(TIMESTAMP);
  Serial.print("AIRTIME:                  ");Serial.println(AIRTIME);
  Serial.print("FULL_REPETITIONS:         ");Serial.println(FULL_REPETITIONS);
  Serial.print("MASTER_CHANNEL:           ");Serial.println(MASTER_CHANNEL);
  Serial.print("SLAVE_CHANNEL:            ");Serial.println(SLAVE_CHANNEL);
  Serial.print("DMX_BROADCASTING:         ");Serial.println(DMX_BROADCASTING);
  Serial.print("CHANNEL_TOTAL:            ");Serial.println(CHANNEL_TOTAL);
  Serial.print("BROADCAST_FRAME_SIZE:     ");Serial.println(BROADCAST_FRAME_SIZE);
  Serial.print("UNICAST_FRAME_SIZE:       ");Serial.println(UNICAST_FRAME_SIZE);
  Serial.print("SEND_REPITITION:          ");Serial.println(SEND_REPITITION);
  Serial.print("WAIT_AFTER_SEND:          ");Serial.println(WAIT_AFTER_SEND);
  Serial.print("WAIT_AFTER_REP_SEND:      ");Serial.println(WAIT_AFTER_REP_SEND);

  // Constants
  Serial.print("MAX_BROADCAST_FRAME_SIZE: ");Serial.println(MAX_BROADCAST_FRAME_SIZE);
  Serial.print("MAX_UNICAST_FRAME_SIZE:   ");Serial.println(MAX_UNICAST_FRAME_SIZE);
  Serial.print("MAX_SLAVES:               ");Serial.println(WAIT_AFTER_REP_SEND);
  return;
}