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

// #include "PythonBridge.h"
#include "MacList.h"

// MASTER CONSTANTS:
// int MAX_BROADCASAT_FRAME_SIZE = 250;
// int MAX_SLAVES            = 20;

esp_now_peer_info_t slaves[MAX_SLAVES] = {};
uint8_t slaveArray[MAX_SLAVES][6]; // magic 6 - MAC has 6 byte
int slaveCount = 1;

// TODO remove
String success;
String puropse = "MASTER";

// init HardwareSerial
HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

// ++ SENDING MESSAGES
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
  uint8_t full_repetitions;
  uint8_t master_channel;
  uint8_t slave_channel;
  uint8_t dmx_broadcasting;
  uint8_t channel_total;
  uint8_t broadcast_frame_size;
  uint8_t unicast_frame_size;
  uint8_t send_repitition;
  uint8_t wait_after_send;
  uint8_t troll = 10;
  uint16_t wait_after_rep_send;
} struct_advanced_meta;

// Master-Slave DMX information BROADCAST
typedef struct struct_dmx_message {
  uint8_t metaCode = 33;
  uint8_t broadcastId; // != 0
  uint8_t dmxFrame[MAX_BROADCAST_FRAME_SIZE];
} struct_dmx_message;

//// Master-Slave Broadcast Meta Information
// // typedef struct struct_dmx_meta {
// //   uint8_t broadcastIdZero;
// //   uint8_t broadcastId;      // != 0
// //   uint8_t broadcastOffset;
// //   // MAC_Address for the node as check, maybe instead of 0 flag for BroadcastID
// // } struct_dmx_meta;

// ++ RECEIVED MESSAGES ++
typedef struct struct_slave_information {
  uint8_t channelCount;
} struct_slave_information;

typedef struct struct_dmx_unicast {
  uint8_t mac[6]; // != 0
  uint8_t dmxFrame[MAX_UNICAST_FRAME_SIZE];
} struct_dmx_unicast;

// Init metadata
// // struct_dmx_meta          dmx_meta;
struct_advanced_meta     advanced_meta;
struct_slave_information slave_information;

int broadcastID;
int slaveoffsets[20];

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[ERROR] Delivery Fail");
  }
  else {
    Serial.println("[OK] Ack");
  }
}

void setup() {
  // Setup Serial
  Serial.begin(115200);
  hSerial.begin(115200);

  Serial.println("Master");

  // Init ESPTimer with a fallback logic
  InitESPTimer();

  // TODO: test - Try to receive data from Peers
  // bool newSlave = 0; // is changed after a new peer appears
  // esp_now_register_recv_cb(onDataRecv);

  delay(1000);
  printSettings();
}

void loop() {
  // TODO Send variable back to computer using JSON
  // get values from python skript serialComunication (SerialControl)
  pythonBridge();
  // parse values from Master into meta package
  createMetaPackage();
  // ESP-NOW-Broadcast (WiFi.mode, InitESP, adding BroadcastPeer, sending meta Information)

  if (DMX_BROADCASTING)
    setupBroadcast();
  else
    setupUnicast();
    
  // TODO send unicast with meta information to each slave
  // for(int i=0; i<1; i++){ // For loop is for iterating through MAC_addresses
    metaInformationToSlaves(BROADCAST_MAC, advanced_meta);
  // }


  // ++++ RUN TEST! ++++
  for (int i = 0; i < FULL_REPETITIONS; i++) {
    if (VERBOSE) {
      Serial.print("Repition "); 
      Serial.println(i);
    }
    // DMX BROADCASTGING TEST
    if (DMX_BROADCASTING) {
      if(TIMESTAMP || AIRTIME) 
        setTimestamp();
      for (int r = 0; r < SEND_REPITITION; r++) {
        // send DMX broadcast to all peers
        sendDataEspBroadcast();
      }
    }
    // DMX UNICASTING TEST
    else { 
      if(TIMESTAMP || AIRTIME) 
        setTimestamp();
      // Send data to device
      for (int r = 0; r < SEND_REPITITION; r++) {
        // send DMX broadcast to all peers
        sendDataEspUnicast();
      }
    }
    // TODO DMX ARTNET TEST
    
    // Collecting timestamps
    if(TIMESTAMP || hSerial.available()) { // aka AIRTIME
      getTimestamp();
      Serial.write(hSerial.read());Serial.println("");
    }
    // wait for shortly to run the sending groups again
    delay(WAIT_AFTER_REP_SEND);
  }
}
