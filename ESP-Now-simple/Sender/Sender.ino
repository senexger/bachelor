/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
// S1: FC:F5:C4:2F:8F:EC
// S2: FC:F5:C4:2F:79:C4
// S3: FC:F5:C4:31:69:0C
// S4: FC:F5:C4:31:5F:44
uint8_t broadcastAddress0[] = {0xFC, 0xF5, 0xC4, 0x2F, 0x8F, 0xEC};
uint8_t broadcastAddress1[] = {0xFC, 0xF5, 0xC4, 0x2F, 0x79, 0xC4};
uint8_t broadcastAddress2[] = {0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C};
uint8_t broadcastAddress3[] = {0xFC, 0xF5, 0xC4, 0x31, 0xF5, 0x44};
uint8_t selectedNode;
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// This is a callback function that will be executed when a message is sent
// In this case, this message simply prints if the message was successfully delivered or not.
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? ". Sender Delivery Success" : ". Sender Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    ESP.restart();
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = "Hello";
  myData.e = false;

  Serial.println(" ================= ");
  for (selectedNode=0; selectedNode<4; selectedNode++) {

    uint8_t * swap;// = broadcastAddress1;

    switch (selectedNode)
    {
      case 0: swap = broadcastAddress0; break;
      case 1: swap = broadcastAddress1; break;
      case 2: swap = broadcastAddress2; break;
      case 3: swap = broadcastAddress3; break;
      default:
        Serial.println("No bradcast address found");
        break;
    }

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(swap, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
      Serial.print(selectedNode); Serial.println(". Receiver Sent with success");
    }
    else {
      Serial.print(selectedNode); Serial.println(". Receiver Error sending data");
    }
  }
  delay(2000);
}
