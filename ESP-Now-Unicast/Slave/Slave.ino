/**
   ESPNOW - Bachelor Thesis Project
   Date: 1th March 2020
   Author: Maximilian W. Gotthardt
   Purpose: DMX implementation via ESP-Now instead of Art-Net
   Description: ...
   Resources: (A bit outdated)
   a. https://espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
   b. http://www.esploradores.com/practica-6-conexion-esp-now/

   << This Device Slave >>

   Flow: Master
   Step 1 : ESPNow Init on Master and set it in STA mode
   Step 2 : Start scanning for Slave ESP32 (we have added a prefix of `slave` to the SSID of slave for an easy setup)
   Step 3 : Once found, add Slave as peer
   Step 4 : Register for send callback
   Step 5 : Start Transmitting data from Master to Slave(s)

   Flow: Slave
   Step 1 : ESPNow Init on Slave
   Step 2 : Update the SSID of Slave with a prefix of `slave`
   Step 3 : Set Slave in AP mode
   Step 4 : Register for receive callback and wait for data
   Step 5 : Once data arrives, print it in the serial monitor

   Note: Master and Slave have been defined to easily understand the setup.
         Based on the ESPNOW API, there is no concept of Master and Slave.
         Any devices can act as master or salve.
*/

#include <esp_now.h>
#include <WiFi.h>

// TODO what defines Channel exactly Master Slave same channel?
#define CHANNEL 1
#define DMX_FRAME_SIZE 250

#define DEBUG 1

#define CHANNEL_NEEDED 14

// ++++ INIT STUFF FOR SLAVE - SENDING ++++
// hardcoded mac from the master
static uint8_t MASTER_MAC[] = {0xFC, 0xF5, 0xC4, 0x31, 0x9A, 0x44};
static uint8_t BROADCAST_MAC[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

typedef struct struct_slavePackage {
    int channel;
} struct_slavePackage;

// Create a struct_slavePackage called slavePackage to send required channels
struct_slavePackage slavePackage;

// Variable to store if sending data was successful
String success;

// ++++ STUFF FOR RECEIVE ++++
typedef struct esp_dmx_message {
  uint8_t payload[DMX_FRAME_SIZE];
} esp_dmx_message;

// Create a struct_message called myData
esp_dmx_message myData;

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
  addBroadcastPeer();
}

void addBroadcastPeer () {
  esp_now_peer_info_t peer_info;
  peer_info.channel = CHANNEL;
  memcpy(peer_info.peer_addr, BROADCAST_MAC, 6);
  peer_info.ifidx = ESP_IF_WIFI_STA;
  peer_info.encrypt = false;
  esp_err_t status = esp_now_add_peer(&peer_info);
  if (ESP_OK != status) { Serial.println("Could not add peer"); }
}

// config AP SSID
void configDeviceAP() {
  String Prefix = "Slave:";
  String Mac = WiFi.macAddress();
  String SSID = Prefix + Mac;
  String Password = "123456789";
  bool result = WiFi.softAP(SSID.c_str(), Password.c_str(), CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

void setup() {
  // Setup Serial
  Serial.begin(115200);
  Serial.println("Slave Node here");

  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());

  // Init ESPNow with a fallback logic
  InitESPNow();

  // // Register peer
  // esp_now_peer_info_t peerInfo;
  // memcpy(peerInfo.peer_addr, BROADCAST_MAC, 6);
  // peerInfo.channel = 0; // TODO: correct channel 
  // peerInfo.encrypt = false;

  // // Add peer not needed for multicast
  // if (esp_now_add_peer(&peerInfo) != ESP_OK){
  //   Serial.println("Failed to add peer");
  //   return;
  // }
  // else { Serial.println("Added master as peer"); }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_send_cb(OnDataSent);
  // Register for a callback function that will be called when data is received
  // esp_now_register_recv_cb(OnDataRecv);
}

// TODO register peer function

// callback when data is recv from Master just printing incomming data
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incommingData, int data_len) {
  memcpy(&myData, incommingData, sizeof(myData));
  Serial.print("Bytes received: "); Serial.print(data_len);
  
  // magic number 20 should be data_len
  bool signalBroken = false;
  for (int i=0; i < data_len; i++) {
    if (myData.payload[i] != i) {
      signalBroken = true;
    }
  }
  if (signalBroken) {
    if(DEBUG) Serial.println(" (F)"); // F stands for False
  }
  else {
    if(DEBUG) Serial.println(" (T)"); // T stands for True
  }
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
  //          mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Last Packet Recv from: "); Serial.println(macStr);
}

// Callback when data is sent - Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// callback when data is sent from Master to Slave
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // char macStr[18];
  // snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
          //  mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // Serial.print("Last Packet Sent to: "); Serial.print(macStr);
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? " Delivery Success" : " Delivery Fail");
  // }

// copied from master
void sendESPBroadcast() {
  if(DEBUG) Serial.println("==== Begin Broadcasts ====");
  esp_err_t broadcastResult = esp_now_send(BROADCAST_MAC, (uint8_t *) &myData, sizeof(myData));
  if (broadcastResult == ESP_OK) {
    Serial.print("Success, Bytes sended: ");
    Serial.println((int) sizeof(myData));
  } else if (broadcastResult == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (broadcastResult == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (broadcastResult == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (broadcastResult == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (broadcastResult == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
  }
}

void loop() {
  // Chill

  // Send message via ESP-NOW
  sendESPBroadcast();

  // wait for incomming messages
  delay(1000);
}
