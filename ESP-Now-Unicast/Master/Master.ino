/**
   ESPNOW - Bachelor Thesis Project
   Date: 1th March 2020
   Author: Maximilian W. Gotthardt
   Purpose: DMX implementation via ESP-Now instead of Art-Net
   Description: ...
   Resources: (A bit outdated)
   a. https://espressif.com/sites/default/files/documentation/esp-now_user_guide_en.pdf
   b. http://www.esploradores.com/practica-6-conexion-esp-now/

   << This Device Master >>

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


  // Sample Serial log with 1 master & 2 slaves
      Found 12 devices 
      1: Slave:24:0A:C4:81:CF:A4 [24:0A:C4:81:CF:A5] (-44)
      3: Slave:30:AE:A4:02:6D:CC [30:AE:A4:02:6D:CD] (-55)
      2 Slave(s) found, processing..
      Processing: 24:A:C4:81:CF:A5 Status: Already Paired
      Processing: 30:AE:A4:2:6D:CD Status: Already Paired
      Sending: 9
      Send Status: Success
      Last Packet Sent to: 24:0a:c4:81:cf:a5
      Last Packet Send Status: Delivery Success
      Send Status: Success
      Last Packet Sent to: 30:ae:a4:02:6d:cd
      Last Packet Send Status: Delivery Success
*/

#include <esp_now.h>
#include <WiFi.h>
#include <esp_timer.h>

#define DEBUG 1

// timestamps
unsigned long timestamp;
unsigned long timediff;

// TODO removeme
String success;

// Global copy of slave
#define NUMSLAVES 20
esp_now_peer_info_t slaves[NUMSLAVES] = {};
int SlaveCnt = 0;

// Broadcast mac 
// for broadcasts the addr needs to be ff:ff:ff:ff:ff:ff
// all devices on the same channel
static uint8_t broadcast_mac[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

esp_now_peer_info_t slave;

#define CHANNEL 7
#define PRINTSCANRESULTS 0

// ESP write message
#define DMX_FRAME_SIZE 250
#define SEND_REPITITION 1
#define ISBROADCASTING 1

typedef struct esp_dmx_message {
  uint8_t dmxFrame[DMX_FRAME_SIZE];
} esp_dmx_message;

esp_dmx_message myData;

void sendESPBroadcast() {
  if(DEBUG) Serial.println("==== Begin Broadcasts ====");
  esp_err_t broadcastResult = esp_now_send(broadcast_mac, (uint8_t *) &myData, sizeof(myData));
  if(DEBUG) espNowStatus(broadcastResult);
}

// send data
void sendESPUnicast() {
  if(DEBUG) Serial.println("==== Begin Unicasts ====");
  for (int i = 0; i < SlaveCnt; i++) {
    const uint8_t *peer_addr = slaves[i].peer_addr;
    Serial.print("Slave "); Serial.print(i); Serial.print(": ");
    esp_err_t unicastResult = esp_now_send(peer_addr, (uint8_t *) &myData, sizeof(myData));
    if(DEBUG) espNowStatus(unicastResult);
  }
}

// callback when data is sent from Master to Slave
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.print(macStr);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? " Delivery Success" : " Delivery Fail");
  }

// Callback when data is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  // if (status ==0){
    // success = "Delivery Success :)";
  // }
  // else{
    // success = "Delivery Fail :(";
  // }
// }

void setup() {
  // setup test data
  for (int i=0; i < DMX_FRAME_SIZE; i++) {
    myData.dmxFrame[i] = i;
  }

  Serial.begin(115200);
  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);
  Serial.println("ESP-Now Master");
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Init ESPTimer with a fallback logic
  InitESPTimer();

  // TODO: test - Try to receive data from Peers
  // esp_now_register_recv_cb(OnDataRecv);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  if(DEBUG) esp_now_register_send_cb(OnDataSent);
}

void loop() {
  esp_dmx_message msg;
  // In the loop we scan for slave
  if (ISBROADCASTING) {
    if(DEBUG) setTimestamp();
    for (int r = 0; r < SEND_REPITITION; r++){
    // send ESP message to each connected peer
    sendESPBroadcast();
    }
    if(DEBUG) getTimestamp();
  }
  else { // UNICASTING
    ScanForSlave();
    // If Slave is found, it would be populate in `slave` variable
    // We will check if `slave` is defined and then we proceed further
    if (SlaveCnt > 0) { // check if slave channel is defined
      // `slave` is defined
      // Add slave as peer if it has not been added already
      manageSlave();
      // pair success or already paired
      // Send data to device
      if(DEBUG) setTimestamp();
      // Send XY packages in a row
      for (int r = 0; r < 10; r++){
        // send ESP message to each connected peer
        sendESPUnicast();
      }
      if(DEBUG) getTimestamp();
    } else {
      // No slave found to process
      if(DEBUG) Serial.println("No slave found.");
    }
  }

  // wait for shortly to run the logic again
  delay(200); //delay(30);
}

static void msg_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len)
{
  if (len == sizeof(esp_dmx_message))
  {
    esp_dmx_message msg;
    memcpy(&msg, data, len);
  }
}

static void msg_send_cb(const uint8_t* mac, esp_now_send_status_t sendStatus)
{
  switch (sendStatus)
  {
    case ESP_NOW_SEND_SUCCESS:
      if(DEBUG) Serial.println("Send success");
      break;

    case ESP_NOW_SEND_FAIL:
      if(DEBUG) Serial.println("Send Failure");
      break;

    default:
      break;
  }
}