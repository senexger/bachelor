/**
   ESPNOW - Broadcast test - Master
*/

#include <esp_now.h>
#include <WiFi.h>
#include <esp_timer.h>

// timestamps
unsigned long timestamp;
unsigned long timediff;

// Global copy of slave
#define NUMSLAVES 20

// From Github:
// Global copy of slave / peer device 
// for broadcasts the addr needs to be ff:ff:ff:ff:ff:ff
// all devices on the same channel
esp_now_peer_info_t slave;

#define CHANNEL 3
#define PRINTSCANRESULTS 0

// Broadcast mac 
static uint8_t broadcast_mac[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

// ESP write message
#define DMX_FRAME_SIZE 20
#define ISBROADCASTING true

typedef struct esp_dmx_message
{
  uint8_t dmxFrame[DMX_FRAME_SIZE];
  // Can put lots of things here...
} esp_dmx_message;

esp_dmx_message myData;

// timestamping
void setTimestamp() {
  timestamp = (unsigned long) (esp_timer_get_time() );
  return;
}
unsigned long getTimestamp() {
  timediff = (unsigned long) (esp_timer_get_time() ) - timestamp;
  Serial.print(timediff); Serial.println(" us");
  return timediff;
}

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow success");
  }
  else {
    Serial.println("ESPNow init failed");
    ESP.restart();
  }

  // add broadcast peer
  esp_now_peer_info_t peer_info;
  peer_info.channel = CHANNEL;
  memcpy(peer_info.peer_addr, broadcast_mac, 6);
  peer_info.ifidx = ESP_IF_WIFI_STA;
  peer_info.encrypt = false;
  esp_err_t status = esp_now_add_peer(&peer_info);
  if (ESP_OK != status)
  {
    Serial.println("Could not add peer");
  }

  // Set up callback TODO: Why?!
  status = esp_now_register_recv_cb(msg_recv_cb);
  if (ESP_OK != status)
  {
    Serial.println("Could not register callback");
  }
  status = esp_now_register_send_cb(msg_send_cb);
  if (ESP_OK != status)
  {
    Serial.println("Could not register send callback");
  }

}

// Init ESP Timer with fallback
void InitESPTimer() {
  if (esp_timer_init() == ESP_OK) {
    Serial.println("ESPTimer Init Success");
  }
  if (esp_timer_init() == ESP_ERR_NO_MEM) {
    Serial.println("ESPTimer allocation has failed");
  }
  if (esp_timer_init() == ESP_ERR_INVALID_STATE) {
    Serial.println("ESPTimer already initialized");
  }
  else {
    Serial.println("ESPTimer Init Failed");
    ESP.restart();
  }
}

static void send_msg(esp_dmx_message * msg)
{
  // Pack
  uint16_t packet_size = sizeof(esp_dmx_message);
  uint8_t msg_data[packet_size];
  memcpy(&msg_data[0], msg, sizeof(esp_dmx_message));

  esp_err_t status = esp_now_send(broadcast_mac, msg_data, packet_size);
  if (ESP_OK != status)
  {
    Serial.println("Error sending message");
  }
}

// send data
void sendData() {
  Serial.println("==== Begin Sending ====");

  // Pack
  // memcpy(&msg_data[0], msg, sizeof(esp_dmx_message));

  Serial.println("Broadcasting");
  esp_err_t result = esp_now_send(broadcast_mac, (uint8_t *) &myData, sizeof(myData));

  // Print status of sended data
  Serial.print("Send Status: ");
  if (result == ESP_OK) {
    Serial.print("Success, Bytes sended: ");
    Serial.println((int) sizeof(myData));
  } else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
    // How did we get so far!!
    Serial.println("ESPNOW not Init.");
  } else if (result == ESP_ERR_ESPNOW_ARG) {
    Serial.println("Invalid Argument");
  } else if (result == ESP_ERR_ESPNOW_INTERNAL) {
    Serial.println("Internal Error");
  } else if (result == ESP_ERR_ESPNOW_NO_MEM) {
    Serial.println("ESP_ERR_ESPNOW_NO_MEM");
  } else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
    Serial.println("Peer not found.");
  } else {
    Serial.println("Not sure what happened");
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

void setup() {
  // setup test data
  for (int i=0; i < DMX_FRAME_SIZE; i++) {
    myData.dmxFrame[i] = i;
  }

  Serial.begin(115200);

  //Set device in STA mode to begin with
  WiFi.mode(WIFI_STA);

  Serial.println("Broadcast ESP-Now Master");
  // This is the mac address of the Master in Station Mode
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Init ESPTimer with a fallback logic
  InitESPTimer();
  // Register for Send CB to
}

static void msg_send_cb(const uint8_t* mac, esp_now_send_status_t sendStatus)
{

  switch (sendStatus)
  {
    case ESP_NOW_SEND_SUCCESS:
      Serial.println("Send success");
      break;

    case ESP_NOW_SEND_FAIL:
      Serial.println("Send Failure");
      break;

    default:
      break;
  }
}

static void msg_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len)
{
  if (len == sizeof(esp_dmx_message))
  {
    esp_dmx_message msg;
    memcpy(&msg, data, len);
  }
}

void loop() {
  // 
  esp_dmx_message msg;
  setTimestamp();
  //send_msg(&msg);
  sendData();
  getTimestamp();

  // wait for three seconds to run the logic again
  delay(3000);
}
