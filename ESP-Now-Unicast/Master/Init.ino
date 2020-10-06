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
  peer_info.channel = CHANNEL;
  memcpy(peer_info.peer_addr, broadcast_mac, 6);
  peer_info.ifidx = ESP_IF_WIFI_STA;
  peer_info.encrypt = false;
  esp_err_t status = esp_now_add_peer(&peer_info);
  if (ESP_OK != status)
  {
    Serial.println("Could not add peer");
  }
}

// Timestamping
void setTimestamp() {
  timestamp = (unsigned long) (esp_timer_get_time() );
  return;
}
unsigned long getTimestamp() {
  timediff = (unsigned long) (esp_timer_get_time() ) - timestamp;
  if (INFO) {
    Serial.print("[INFO] "); Serial.print(timediff); Serial.println(" us");
  }
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
    Serial.print("[OK] Send: ");
    Serial.print((int) sizeof(dmxData));
    Serial.println(" B");
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