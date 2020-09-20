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
  Serial.begin(115200);
  Serial.println("Slave Node is ready");

  //Set device in AP mode to begin with
  WiFi.mode(WIFI_AP);
  // configure device AP mode - not needed anymore using ESP-Unicast
  // configDeviceAP();

  // This is the mac address of the Slave in AP Mode
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  
  // Init ESPNow with a fallback logic
  InitESPNow();

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_send_cb(OnDataSent);
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, BROADCAST_MAC, 6);
  peerInfo.channel = 0; // TODO: correct channel 
  peerInfo.encrypt = false;
  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  else { Serial.println("Added master as peer"); }
}

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

// Callback when data is sent - we are in the Slave node!
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

void subscribeToMaster() {
  return;
}

void loop() {
  // Chill
  Serial.println("i am here...");
  //esp_err_t result = esp_now_send(MASTER_MAC, (uint8_t *) &slavePackage, sizeof(slavePackage));

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(BROADCAST_MAC, (uint8_t *) &slavePackage, sizeof(slavePackage));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}
