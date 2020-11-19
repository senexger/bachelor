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

// TODO what defines Channel exactly Master Slave same channel?
#define CHANNEL 1
#define DMX_FRAME_SIZE 200

// Two level debug information
#define DEBUG   1
#define VERBOSE 0

#define CHANNELS_NEEDED 14

// ++++ INIT STUFF FOR SLAVE - SENDING ++++
#define IS_BROADCAST 0

#if IS_BROADCAST
  static uint8_t MAC_ADDRESS[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#else 
  static uint8_t MAC_ADDRESS[] = {0xFC, 0xF5, 0xC4, 0x31, 0x9A, 0x44};
#endif

// Variable to store if sending data was successful
String success;

uint8_t broadcastId;
uint8_t offset;

// ++++ STUFF FOR RECEIVE ++++
typedef struct struct_dmx_data {
  uint8_t broadcastID;
  uint8_t payload[DMX_FRAME_SIZE];
} struct_dmx_data;

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

// Create a struct_message called dmxData
struct_dmx_data dmxData;
int thisBroadcastID;
int thisBroadcastOffset;
// Create a struct_slavePackage called slavePackage to send required channels
struct_slave_requirements slaveRequirements;
// create struct for the broadcast offsets and IDs
struct_dmx_meta dmxMeta;
bool isDmxMetaReceived = 0;

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }

  addPeer(MAC_ADDRESS);
}

void addPeer (uint8_t mac_address[6]) {
  esp_now_peer_info_t peer_info;
  peer_info.channel = CHANNEL;
  memcpy(peer_info.peer_addr, mac_address, 6);
  peer_info.ifidx = ESP_IF_WIFI_STA;
  peer_info.encrypt = false;
  esp_err_t status = esp_now_add_peer(&peer_info);
  if (ESP_OK != status) { Serial.println("Could not add peer"); }
}

void setup() {
  // Setup test data
  slaveRequirements.channelCount = CHANNELS_NEEDED;
  
  // Setup Serial
  Serial.begin(115200);
  Serial.println("Slave Node here");

  //Set device in AP mode to begin with
  WiFi.mode(WIFI_STA);
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  if(DEBUG) esp_now_register_send_cb(OnDataSent);
  // Register for a callback function that will be called when data is received
  if(DEBUG) esp_now_register_recv_cb(OnDataRecv);
}

// callback when data is recv from Master just printing incomming data
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  // memcpy(&dmxMeta, incommingData, sizeof(dmxMeta));
  memcpy(&dmxData, incommingData, sizeof(dmxData));
  
  // check if the package is a DMXData or DMXMeta package
  if (dmxData.broadcastID == 0) {
    // memcpy(&dmxMeta, incommingData, sizeof(dmxMeta));
    if(DEBUG) {
      Serial.print("DmxBit: "); Serial.println(dmxData.broadcastID);
      Serial.print("ID    : "); Serial.println(dmxData.payload[0]);
      Serial.print("Offset: "); Serial.println(dmxData.payload[1]);
    }
    broadcastId = dmxData.payload[0];
    offset = dmxData.payload[1];
    // meta data received, so dont aks for them anymore in the loop
    isDmxMetaReceived = 1;
  }
  // package containing DMX Data
  else {
    Serial.print("[Info] dmx broadcast ID: "); Serial.println(dmxData.broadcastID);

    // magic number 20 should be data_len
    bool signalBroken = false;

    // check broadcast integrity
    Serial.print("BroadcastID :"); Serial.println(dmxData.broadcastID);
    // sub 1 becaus there is no broadcastID in payload
    // iterating through the payload
    if (dmxData.broadcastID == broadcastId) {
      for (int i=1; i < data_len -1; i++) { 
        // just select needed channel
        // if (VERBOSE) {
        if ((i >= offset) && (i < offset + CHANNELS_NEEDED)) {
          Serial.print(i); 
          Serial.print(" = "); 
          Serial.println(dmxData.payload[i]); 
        }
        if (dmxData.payload[i] != i) {
          signalBroken = true;
        }
      }
    }

    if (signalBroken) {
      if(DEBUG) {
        Serial.print("[ERROR] Incomming Data broken: "); 
        Serial.print(data_len);
        Serial.println(" B");
      }
    }
    else {
      if(DEBUG) { 
        Serial.print("[OK] Rcvd: "); 
        Serial.print(data_len);
        Serial.println(" B");
      }
    }
  }
}

// Callback when data is sent - Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.println("Last Packet Send Status");
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[ERROR] Delivery Fail");
  }
  else {
    Serial.println("[OK] Send Ack");
  }
}

// copied from master
void sendESPCast(uint8_t mac_address[6]) {
  if(VERBOSE && IS_BROADCAST) Serial.println("[Info] Begin BROADCAST");
  if(VERBOSE && !IS_BROADCAST) Serial.println("[Info] Begin UNICAST");
  esp_err_t broadcastResult = 
        esp_now_send(mac_address, (uint8_t *) &slaveRequirements, sizeof(slaveRequirements));
  if (broadcastResult == ESP_OK) {
    Serial.print("[OK] Send: ");
    Serial.print((int) sizeof(slaveRequirements));
    Serial.println(" B");
  } else if (broadcastResult == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("[ERROR] ESPNOW not Init.");
  } else if (broadcastResult == ESP_ERR_ESPNOW_ARG) {
    Serial.println("[ERROR] Invalid Argument");
  } else if (broadcastResult == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("[ERROR] Internal Error");
  } else if (broadcastResult == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("[ERROR] ESP_ERR_ESPNOW_NO_MEM");
  } else if (broadcastResult == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("[ERROR] Peer not found.");
  } else {
    Serial.println("[ERROR] Not sure what happened");
  }
}

void loop() {
  // Chill

  // Send message via ESP-NOW
  if (!isDmxMetaReceived) { sendESPCast(MAC_ADDRESS); }

  // wait for incomming messages
  delay(1000);
}

// config AP SSID
// void configDeviceAP() {
  // String Prefix = "Slave:";
  // String Mac = WiFi.macAddress();
  // String SSID = Prefix + Mac;
  // String Password = "123456789";
  // bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  // if (!result) {
    // Serial.println("AP Config failed.");
  // } else {
    // Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  // }
// }