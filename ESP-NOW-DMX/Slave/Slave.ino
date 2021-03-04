/*
   ESPNOW-SLAVE
   Date: 1th March 2020
   Author: Maximilian W. Gotthardt
   Purpose: DMX implementation via ESP-Now
*/

#include <esp_now.h>
#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>
#include "MacList.h"

#define CHANNELS_NEEDED 6

// TODO: Seems to bee wrong. Should also be spreaded via python bridge
#if DMX_BROADCASTING
  static uint8_t MAC_ADDRESS[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#else 
  static uint8_t MAC_ADDRESS[] = {0xFC, 0xF5, 0xC4, 0x31, 0x9A, 0x44};
#endif

// Variable to store if sending data was successful
String success;
String puropse = "SLAVE";

// Variables for sending and receiving
bool isDmxMetaReceived = 0;
uint8_t broadcastId;
uint8_t offset;

// UART checking
HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

// ++++ STUFF FOR RECEIVE ++++

// Package including a ID to tell the package contaings meta or data and if data
typedef struct struct_esp_data_broadcast {
  uint8_t metaCode;
  uint8_t broadcastID;
  uint8_t payload[MAX_BROADCAST_FRAME_SIZE];
} struct_esp_data_broadcast;

// Package including just the ESP-Unicast information
typedef struct struct_esp_data_unicast {
  uint8_t payload[MAX_BROADCAST_FRAME_SIZE];
} struct_esp_data_unicast;

// // // gives the slave the information where to find his channel
// // typedef struct struct_dmx_meta {
// //   uint8_t isMetaData; // should set to -1
// //   uint8_t broadcastID; // must be positive
// //   uint8_t broadcastOffset;
// // } struct_dmx_meta;

// new architecture 
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

// ++++ STUFF FOR SENDING ++++
typedef struct struct_slave_requirements {
  // TODO: macaddresse uint8_t ?
  // TODO: semi informations
  // uint8_t packageID;
  // uint8_t slaveID;
  uint8_t channelCount;
} struct_slave_requirements;

// Create a struct_message called espBroadcastData
struct_esp_data_broadcast espBroadcastData;
struct_advanced_meta      advanced_Meta;
struct_esp_data_unicast   espUnicastData;

int thisBroadcastID;
int thisBroadcastOffset;
// Create a struct_slavePackage called slavePackage to send required channels
struct_slave_requirements slaveRequirements;
// create struct for the broadcast offsets and IDs
// // struct_dmx_meta dmx_meta;
struct_advanced_meta dmx_meta;

void setup() {
  // Setup test data
  slaveRequirements.channelCount = CHANNELS_NEEDED;
  
  // Setup Serial
  Serial.begin(115200);
  hSerial.begin(115200); // open Serial Port to the Master RX2 TX2 GND
  Serial.println("Slave Node here");

  Serial.println("init success!");
  // addPeer(MAC_ADDRESS);

  // Just setup a default ESP mode
  setupEspNow();
}

void loop() {
  // Serial2.println("A");

  printSettings();

  // // Send message via ESP-NOW if MetaData wasn't received
  // // Wäre gut, wenn man das hier gar nicht benötigen würde...
  // if (!isDmxMetaReceived && DMX_BROADCASTING) 
  //   sendESPCast(MAC_ADDRESS);
  // else if (VERBOSE) 
  //   Serial.println("DMX Meta already received");

  // wait for incomming messages
  delay(10000);
}

// callback when data is recv from Master just printing incomming data
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  if(VERBOSE) Serial.println("VERBOSE: OnDataRecv()");
  // TODO ist das sinnvoll?!
  if (AIRTIME) {
    Serial2.print("!");
  }
  Serial.print("Data len: "); Serial.println(data_len);

  // META PACKAGE HANDLING
  if (incommingData[0]) { // == advanced_Meta.metaCode from master struct
    applyMetaInformation(incommingData, data_len);
  }
  // DATA PACKAGE HANDLING
  else {
    if (checkPayload(incommingData, data_len)) {
      if(DEBUG) { 
        Serial.print("[OK] Rcvd: "); 
        Serial.print(data_len);
        Serial.println(" B (not broken)");
       // TODO: timestamp here...
      }
    }
    else {
      if(DEBUG) {
        Serial.print("[ERROR] Incomming Data broken: "); 
        Serial.print(data_len);
        Serial.println(" B");
      }
    }
    // TODO: sure?
    getTimestamp();
    setTimestamp();
  }
}

void setupEspNow() {
  //Set device in AP mode to begin with
  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}