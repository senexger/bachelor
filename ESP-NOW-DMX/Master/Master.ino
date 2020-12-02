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
#include <WiFi.h>
#include <esp_timer.h>
#include "MacList.h"

#define VERBOSE 0
#define DEBUG 0
#define TIMESTAMP 1 // taking timestamps

// timestamps
unsigned long timestamp;
unsigned long timediff;

// TODO removeme
String success;

// peerlist information
esp_now_peer_info_t peer_info;
// Global copy of slave
#define MAX_SLAVES 20
esp_now_peer_info_t slaves[MAX_SLAVES] = {};
uint8_t slaveArray[MAX_SLAVES][6];
int slaveCnt = 0;
// TODO:
// uint8_t slaveMacList;

esp_now_peer_info_t slave;

#define CHANNEL 7
#define PRINTSCANRESULTS 0

// ESP write message
#define DMX_BROADCASTING 1 // else: Unicast
#define CHANNEL_TOTAL 1000
#define DMX_FRAME_SIZE 200
#define SEND_REPITITION 10
#define WAIT_AFTER_SEND 3 // ms


typedef struct struct_dmx_message {
  uint8_t broadcastID; // != 0
  uint8_t dmxFrame[DMX_FRAME_SIZE];
} struct_dmx_message;

// gives the slave the information where to find his channel
typedef struct struct_dmx_meta {
  uint8_t broadcastIdZero;
  uint8_t broadcastID; // != 0
  uint8_t broadcastOffset;
  // MAC_Address for the node as check, maybe instead of 0 flag for BroadcastID
} struct_dmx_meta;

// ++ RECEIVED MESSAGES ++
typedef struct struct_slave_information {
  uint8_t channelCount;
} struct_slave_information;

// Setup dmx broadcast messages
struct_dmx_message dmxData1;
struct_dmx_message dmxData2;
struct_dmx_message dmxData3;
struct_dmx_message dmxData4;
struct_dmx_message dmxData5;
struct_dmx_message broadcastArray[5];

struct_dmx_meta dmxMeta;
struct_slave_information slave_information;

int broadcastID;
int slaveoffsets[20];

void sendDmxBroadcast() {
  if(VERBOSE) Serial.println("[Info] Init DMX Broadcasting");

  for (int j = 0; j < CHANNEL_TOTAL; j+=DMX_FRAME_SIZE) {
    int i = j/DMX_FRAME_SIZE;
    if(DEBUG) { 
      Serial.print("[OK] DMX Broadcast "); 
      Serial.println(broadcastArray[i].broadcastID); }
    esp_err_t broadcastResult = esp_now_send(broadcast_mac, (uint8_t *) &broadcastArray[i], sizeof(broadcastArray[i]));
    if(DEBUG) espNowStatus(broadcastResult);
    delay(WAIT_AFTER_SEND); // No delay crashs the system
  }

  // if(DEBUG) { Serial.println("[OK] DMX Broadcast 2"); }
  // esp_err_t broadcastResult2 = esp_now_send(broadcast_mac, (uint8_t *) &dmxData2, sizeof(dmxData2));
  // if(DEBUG) espNowStatus(broadcastResult2);
// 
  // delay(WAIT_AFTER_SEND); // No delay crashs the system
// 
  // if(DEBUG) { Serial.println("[OK] DMX Broadcast 3"); }
  // esp_err_t broadcastResult3 = esp_now_send(broadcast_mac, (uint8_t *) &dmxData3, sizeof(dmxData3));
  // if(DEBUG) espNowStatus(broadcastResult3);
// 
  // delay(WAIT_AFTER_SEND); // No delay crashs the system
// 
  // if(DEBUG) { Serial.println("[OK] DMX Broadcast 4"); }
  // esp_err_t broadcastResult4 = esp_now_send(broadcast_mac, (uint8_t *) &dmxData4, sizeof(dmxData4));
  // if(DEBUG) espNowStatus(broadcastResult4);
  // 
  // delay(WAIT_AFTER_SEND); // No delay crashs the system
}

// send meta Data to Slave with BroadcastID and Offset
void sendUnicastToMac(const uint8_t *peer_addr, struct_dmx_meta metaData) {
  if(VERBOSE) {
    Serial.print("[Info] Send DMX Information ");
    Serial.print((int) sizeof(metaData));
    Serial.println(" (B)");
  }
  esp_err_t unicastResult = esp_now_send(peer_addr, (uint8_t *) &metaData, sizeof(metaData));
  if(DEBUG) espNowStatus(unicastResult);
}

// send data
// TODO: Think about structs data1, data2, etc
void sendESPUnicast() {
  if(VERBOSE) Serial.println("[Info] DMX Unicasting");
  for (int i = 0; i < slaveCnt; i++) {
    const uint8_t *peer_addr = slaves[i].peer_addr;
    Serial.print("Slave "); Serial.print(i); Serial.print(": ");
    esp_err_t unicastResult = esp_now_send(peer_addr, (uint8_t *) &dmxData1, sizeof(dmxData1));
    if(DEBUG) espNowStatus(unicastResult);
  }
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[ERROR] Delivery Fail");
  }
  else {
    // Serial.println("[OK] Rcvd: Ack");
  }
}

void setup() {
  // Setup test data
  for (int i=1; i < DMX_FRAME_SIZE +1; i++) { dmxData2.dmxFrame[i] = i; }
  dmxData2.broadcastID = 2;
  broadcastArray[0] = dmxData2;
  for (int i=1; i < DMX_FRAME_SIZE +1; i++) { dmxData3.dmxFrame[i] = i; }
  dmxData3.broadcastID = 3;
  broadcastArray[1] = dmxData3;
  for (int i=1; i < DMX_FRAME_SIZE +1; i++) { dmxData4.dmxFrame[i] = i; }
  dmxData4.broadcastID = 4;
  broadcastArray[2] = dmxData4;
  for (int i=1; i < DMX_FRAME_SIZE +1; i++) { dmxData5.dmxFrame[i] = i; }
  dmxData5.broadcastID = 5;
  broadcastArray[3] = dmxData5;

  // Setup Serial
  Serial.begin(115200);
  Serial.println("ESP-Now Master");

  // Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // Init ESPTimer with a fallback logic
  InitESPTimer();

  // TODO: test - Try to receive data from Peers
  bool newSlave = 0; // is changed after a new peer appears
  esp_now_register_recv_cb(OnDataRecv);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  if(DEBUG) esp_now_register_send_cb(OnDataSent);
}

void loop() {
  // TODO:
  // peer should be already added in callback function befor the newSlave flag is changed
  // if (newSlave) {
    // calculate new broadcasts
    // for (each slave) { tell each slave the bradcastID and offset }

  // Serial.print("[Info] Slave Count = ");
  // Serial.println(slaveCnt);

  // TODO: remove 1, or remove hole if?
  // if ( slaveCnt != 0 || 1 ) {
    // In the loop we scan for slave

  if (DMX_BROADCASTING) {
    if(TIMESTAMP) setTimestamp();
    for (int r = 0; r < SEND_REPITITION; r++){
      // send DMX broadcast to all peers
      sendDmxBroadcast(); // TODO: give parameter msg
    }
    if(TIMESTAMP) getTimestamp();
  }
  /*
  else { 
    // UNICASTING
    // TODO add slaves
    manageSlave();
    // Send data to device
    if(DEBUG) setTimestamp();
    // Send XY packages in a row
    for (int r = 0; r < 10; r++){
      // send ESP message to each connected peer
      sendESPUnicast();
    }
    if(DEBUG) getTimestamp();
  }
  */ 
  // }
    // wait for shortly to run the logic again
}

// callback when data is recv from Slave
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  memcpy(&slave_information, incommingData, sizeof(slave_information));
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

  /*
  // saving incomming MACs in a Array for later use
  uint8_t sameEntries;

  // print hole array
  for (int slaveRaw = 0; slaveRaw < MAX_SLAVES; slaveRaw ++) {

    sameEntries = 0;

    // Iterate trough MAC
    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", slaveArray[slaveRaw][i]); if (i < 5)Serial.print(":");
      // Check if exists
      if (slaveArray[slaveRaw][i] == mac_addr[i]){
        sameEntries ++;
      }
    }
    Serial.println("");

    // Serial.print(" Same entries: "); Serial.println(sameEntries);
    if (sameEntries == 6) { Serial.println("[warning] MAC already exist"); break; }
  }

  // Write mac in slaveArray
   if (sameEntries < 6) {
    for (int i = 0; i < 6; i++) {
      slaveArray[slaveCnt][i] = mac_addr[i];
    }
    Serial.println("[OK] Slave added");
    slaveCnt ++;
  }
  */

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

  // TODO: Get values from MACList!!!
  dmxMeta.broadcastIdZero = 0;
  dmxMeta.broadcastID     = 2; // no id becaus its the information channel
  dmxMeta.broadcastOffset = 20;

  // send a massage back with the slave information
  if (DEBUG) {
    Serial.println("[info] send unicast back");
    Serial.print("broadcastId:     "); Serial.println(dmxMeta.broadcastID);
    Serial.print("broadcastoffset: "); Serial.println(dmxMeta.broadcastOffset);
  }

  sendUnicastToMac(mac_addr, dmxMeta);

  // remove slave (needed?!) - not for DMX-Unicast!
  // esp_err_t status2 = esp_now_del_peer(&peer_info);
  // if (ESP_OK != status2) { Serial.println("Could not remove peer"); }
  // else { Serial.println("Slave-peer removed successfully"); }
}