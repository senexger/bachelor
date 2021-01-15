/**
   ESPNOW - Bachelor Thesis Project
   Date: 1th March 2020
   Author: Maximilian W. Gotthardt
   Purpose: DMX implementation via ESP-Now instead of Art-Net

   << This Device Master >>

   Flow: Master Broadcast
   Step 1 : ESPNow Init on Master and set it in STA mode
   Step 2 : Waiting for Unicast messages from slaves
   Step 3 : Adding slave information
   Step 4 : calculate broadcast information
   Step 5 : Send Slave Unicast information for broadcasts
   Step 6 : Send broadcasts

   Flow: Master Unicast answer
   Step 1 : ESPNow Init on Master and set it in STA mode
   Step 2 : Waiting for Unicast messages from slaves
   Step 3 : Adding slave information
   Step 4 : Send Unicast dmx information to each slave

   Flow: Slave
   Step 1 : ESPNow Init on Slave
   Step 2 : Update the SSID of Slave with a prefix of `slave`
   Step 3 : Set Slave in AP mode
   Step 4 : Register for receive callback and wait for data
   Step 5 : Once data arrives, print it in the serial monitor
*/

// TODO: Slave -> Fixture better naming

#include <esp_now.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <esp_timer.h>
#include "MacList.h"

esp_now_peer_info_t slaves[MAX_SLAVES] = {};
uint8_t slaveArray[MAX_SLAVES][6]; // magic 6 - MAC has 6 byte
int slaveCount = 1;

// depricated
// esp_now_peer_info_t slave;

// peerlist information
esp_now_peer_info_t peer_info;

// timestamps
unsigned long timestamp;
unsigned long timediff;

// TODO remove
String success;

// init HardwareSerial
HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

// ++ SENDING MESSAGES
// Master-Slave DMX information BROADCAST
typedef struct struct_dmx_message {
  uint8_t broadcastId; // != 0
  uint8_t dmxFrame[BROADCAST_FRAME_SIZE];
} struct_dmx_message;

// Master-Slave Broadcast Meta Information
typedef struct struct_dmx_meta {
  uint8_t broadcastIdZero;
  uint8_t broadcastId; // != 0
  uint8_t broadcastOffset;
  // MAC_Address for the node as check, maybe instead of 0 flag for BroadcastID
} struct_dmx_meta;

// ++ RECEIVED MESSAGES ++
typedef struct struct_slave_information {
  uint8_t channelCount;
} struct_slave_information;

typedef struct struct_dmx_unicast {
  uint8_t mac[6]; // != 0
  uint8_t dmxFrame[UNICAST_FRAME_SIZE];
} struct_dmx_unicast;

// Setup dmx broadcast messages
struct_dmx_message broadcastData1;
struct_dmx_message broadcastData2;
struct_dmx_message broadcastData3;
struct_dmx_message broadcastData4;
struct_dmx_message broadcastData5;
struct_dmx_message broadcastArray[5];

// Setup dmx unicast messages
struct_dmx_unicast unicastData1;
struct_dmx_unicast unicastData2;
struct_dmx_unicast unicastData3;
struct_dmx_unicast unicastData4;
struct_dmx_unicast unicastData5;
struct_dmx_unicast unicastData6;
struct_dmx_unicast unicastData7;
struct_dmx_unicast unicastData8;
struct_dmx_unicast unicastData9;
struct_dmx_unicast unicastDataArray[9];

// Init metadata
struct_dmx_meta dmx_meta;
struct_slave_information slave_information;

int broadcastID;
int slaveoffsets[20];

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[ERROR] Delivery Fail");
  }
  else {
    // Serial.println("[OK] Rcvd: Ack");
  }
}

void setup() {
  // Setup test data
  // createData();
  if (DMX_BROADCASTING)
    setupBroadcast();
  else 
    setupUnicast();

  // Setup Serial
  Serial.begin(115200);
  hSerial.begin(115200);

  Serial.println("Master");
  // TODO also print Constants like ISBROADCAST, CHANNEL_TOTAL...

  // TODO SetupSlave
  // Set device in STA mode to begin with
  // WiFi.mode(WIFI_STA);
  // Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  // InitESPNow();

  // Init ESPTimer with a fallback logic
  InitESPTimer();

  // TODO: test - Try to receive data from Peers
  // bool newSlave = 0; // is changed after a new peer appears
  // esp_now_register_recv_cb(onDataRecv);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  if(DEBUG) esp_now_register_send_cb(onDataSent);
  delay(1000);
}

void loop() {
  if (DMX_BROADCASTING) {
    if(TIMESTAMP || AIRTIME) setTimestamp();
    for (int r = 0; r < SEND_REPITITION; r++){
      // send DMX broadcast to all peers
      sendDmxBroadcast(); // TODO: give parameter msg
    }
  }
  else { 
    if(TIMESTAMP || AIRTIME) setTimestamp();
    // Send data to device
    for (int r = 0; r < SEND_REPITITION; r++){
      // send DMX broadcast to all peers
      sendESPUnicast();
    }
  }

  // Collecting timestamps
  if(TIMESTAMP || hSerial.available()) { // aka AIRTIME
    getTimestamp();
    Serial.write(hSerial.read());Serial.println("");
  }
    
  // wait for shortly to run the sending groups again
  delay(WAIT_AFTER_REP_SEND);
}
