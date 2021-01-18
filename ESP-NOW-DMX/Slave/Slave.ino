/**
   ESPNOW - Bachelor Thesis Project
   Date: 1th March 2020
   Author: Maximilian W. Gotthardt
   Purpose: DMX implementation via ESP-Now instead of Art-Net
   Description: ...

   << This Device Slave >>

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

#include <esp_now.h>
#include <WiFi.h>
#include <esp_timer.h>
#include <HardwareSerial.h>
#include "Maclist.h"

#define CHANNELS_NEEDED 6

#if DMX_BROADCASTING
  static uint8_t MAC_ADDRESS[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#else 
  static uint8_t MAC_ADDRESS[] = {0xFC, 0xF5, 0xC4, 0x31, 0x9A, 0x44};
#endif

// Variable to store if sending data was successful
String success;

// Variables for sending and receiving
bool isDmxMetaReceived = 0;
uint8_t broadcastId;
uint8_t offset;

// UART checking
HardwareSerial &hSerial = Serial2; //can be Serial2 as well, just use proper pins

// ++++ STUFF FOR RECEIVE ++++

// Package including a ID to tell the package contaings meta or data and if data
typedef struct struct_esp_data_broadcast {
  uint8_t broadcastID;
  uint8_t payload[BROADCAST_FRAME_SIZE];
} struct_esp_data_broadcast;

// Package including just the ESP-Unicast information
typedef struct struct_esp_data_unicast {
  uint8_t payload[BROADCAST_FRAME_SIZE];
} struct_esp_data_unicast;

// gives the slave the information where to find his channel
typedef struct struct_dmx_meta {
  uint8_t isMetaData; // should set to -1
  uint8_t broadcastID; // must be positive
  uint8_t broadcastOffset;
} struct_dmx_meta;

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
struct_esp_data_unicast   espUnicastData;

int thisBroadcastID;
int thisBroadcastOffset;
// Create a struct_slavePackage called slavePackage to send required channels
struct_slave_requirements slaveRequirements;
// create struct for the broadcast offsets and IDs
struct_dmx_meta dmx_meta;

void setup() {
  // Setup test data
  slaveRequirements.channelCount = CHANNELS_NEEDED;
  
  // Setup Serial
  Serial.begin(115200);
  hSerial.begin(115200); // open Serial Port to the Master RX2 TX2 GND
  Serial.println("Slave Node here");

  if (DMX_BROADCASTING)
    setupSlaveBroadcasting();
  else
    setupSlaveUnicast();

  // TODO move to SlaveBroadcast too 
  // //Set device in AP mode to begin with
  // WiFi.mode(WIFI_STA);
  // Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // // Init ESPNow with a fallback logic
  // InitESPNow();
  // addPeer(MAC_ADDRESS);

  // get recv packer info.
  esp_now_register_send_cb(OnDataSent);
}

void loop() {
  // Chill

  // Send message via ESP-NOW if MetaData wasn't received
  if (!isDmxMetaReceived && DMX_BROADCASTING) 
    sendESPCast(MAC_ADDRESS);
  else if (VERBOSE) 
    Serial.println("DMX Meta already received");

  // wait for incomming messages
  delay(1000);
}

// config AP SSID
// void configDeviceAP() {
  // String Prefix = "Slave:"
  // String Mac = WiFi.macAddress();
  // String SSID = Prefix + Mac;
  // String Password = "123456789";
  // bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), SLAVE_CHANNEL, 0);
  // if (!result) {
    // Serial.println("AP Config failed.");
  // } else {
    // Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  // }
// }
