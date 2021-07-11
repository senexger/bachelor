
/*
   ESPNOW-Master
   Date: 1th March 2020
   Author: Maximilian W. Gotthardt
   Purpose: DMX implementation via ESP-Now
*/

// TODO: Slave -> Fixture better naming

#include <esp_now.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <esp_timer.h>
#include "ArduinoJson.h"

#include "MacList.h"

esp_now_peer_info_t slaves[MAX_SLAVES] = {};
uint8_t slaveArray[MAX_SLAVES][6]; // magic 6 - MAC has 6 byte
uint8_t success = 0; // for the 100% reliable unicast
const uint8_t *toCollectFromMac = BROADCAST_MAC; // for the 100% reliable unicast

String puropse = "MASTER";

// using pointer would be more beautiful
unsigned long timestamp;
unsigned long timediff;
unsigned long timestampS[200];
unsigned long timediffS[200];

HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

// ++ SENDING MESSAGES ++
typedef struct struct_advanced_meta {
  uint8_t metaCode;
  // information about broadcast
  uint8_t broadcastId;
  uint8_t broadcastOffset;
  // general information about the next test
  uint8_t slave_offset;
  uint8_t slave_broadcastId;
  uint8_t verbose;
  uint8_t debug;
  uint8_t timestamp;
  uint8_t airtime;
  uint16_t full_repetitions;
  uint8_t master_channel;
  uint8_t slave_channel;
  uint8_t dmx_broadcasting;
  uint16_t channel_total;
  uint8_t broadcast_frame_size;
  uint8_t unicast_frame_size;
  uint8_t SLAVE_COUNT;
  uint8_t rapid_repitition;
  uint16_t send_repitition;
  uint16_t wait_after_send;
  uint16_t wait_after_rep_send;
} struct_advanced_meta;

// Master-Slave DMX information BROADCAST
typedef struct struct_broadcast_message {
  uint8_t metaCode = 253;
  uint8_t broadcastId;
  uint8_t dmxFrame[MAX_BROADCAST_FRAME_SIZE];
} struct_broadcast_message;

// Init metadata
struct_advanced_meta advanced_meta;

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[ERROR] Delivery Fail (On Data Send");
  }
  else {
    Serial.println("[OK] Send Success (On Data Send)");
  }
}

void setup() {
  // Setup Serial
  Serial.begin(115200);
  hSerial.begin(115200);

  Serial.println("Master");

  // Init ESPTimer with a fallback logic
  InitESPTimer();

  delay(1000);
  printSettings();
}

void loop() {
  // ++ PREPARE ++
  // TODO Send variable back to computer using JSON
  pythonBridge();
  createMetaPackage();

  // ++ SETUP ++
  if (DMX_BROADCASTING)
    setupBroadcast();
  else
    setupUnicast();
    
  addNodeToPeerlist(SLAVE_MAC_1);
  addNodeToPeerlist(SLAVE_MAC_2);
  addNodeToPeerlist(SLAVE_MAC_3);
  addNodeToPeerlist(SLAVE_MAC_4);
  addNodeToPeerlist(SLAVE_MAC_6);
  addNodeToPeerlist(SLAVE_MAC_7);
  addNodeToPeerlist(SLAVE_MAC_8);
  addNodeToPeerlist(SLAVE_MAC_9);
  
  esp_now_register_recv_cb(OnDataRecv);

  // ++ DISTRIBUTE ++
  // Distribution with unicast, because its fast and reliable. Using WLAN would be also an option
  // but its rather cumbersome
  Serial.println("===SEND META INFORMATION===");
  for(int i=0; i < SLAVE_COUNT; i++){ // For loop is for iterating through MAC_addresses
    metaInformationToSlaves(SLAVE_MAC_ARRAY[i+1], advanced_meta);
  }

  // ++ TEST ++
  for (int i = 0; i < FULL_REPETITIONS; i++) {
    if (VERBOSE) {
      Serial.print("Full Repition "); 
      Serial.println(i);
    }
    Serial.println("===RUN TEST===");
    runTest();
  }
  Serial.println("===FINITO===");
}

void runTest() {
  // DMX BROADCASTGING TEST
  if (DMX_BROADCASTING) {
    // if(TIMESTAMP || AIRTIME) 
    //   setTimestamp();
    for (uint8_t r = 0; r < SEND_REPITITION; r++) {
      for (uint8_t rr = 1; rr <= RAPID_REPITITION; rr++) {
        sendDataEspBroadcast(r, rr);
      }
    }
  }
  // DMX UNICASTING TEST
  else { 
    // if(TIMESTAMP || AIRTIME) 
    //   setTimestamp();
    for (uint8_t r = 0; r < SEND_REPITITION; r++) {
      sendDataEspUnicast(r);
    }
  }
  // TODO DMX ARTNET TEST

  // Collecting timestamps
  // if(TIMESTAMP || hSerial.available()) { // aka AIRTIME
  //   getTimestamp();
  //   Serial.write(hSerial.read());Serial.println("");
  // }
  // wait for shortly to run the sending groups again
  Serial.println("===COLLECT DATA===");
  for(int i=0; i < SLAVE_COUNT; i++){ // For loop is for iterating through MAC_addresses
    toCollectFromMac = SLAVE_MAC_ARRAY[i+1];
    collectData(SLAVE_MAC_ARRAY[i+1], advanced_meta);
  }
  delay(WAIT_AFTER_REP_SEND);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int data_len) {
  Serial.print("DATA RECEIVED ("); Serial.print(data_len); Serial.print("B) from: "); printMac(mac_addr);

  if (data_len == 1) {
    Serial.println("Ack received!");
    success = 1;
    return;
  }

  Serial.print("To collect from Mac: "); printMac(toCollectFromMac);

  if (*toCollectFromMac != *mac_addr) { 
    Serial.println("Abort Collection"); 
    return;
  }
  toCollectFromMac = BROADCAST_MAC;

  for (int i=0; i< data_len; i++) {
    Serial.println(incomingData[i]);
  }

  Serial.println("Timestamps");
  for (int i = 0; i<SEND_REPITITION; i++) {
    Serial.println(timestampS[i]);
  }

  for (int i=3; i < 6; i++) {
    Serial.print(mac_addr[i]);
  }
  Serial.println("9999");
  Serial.println("DONE");
}