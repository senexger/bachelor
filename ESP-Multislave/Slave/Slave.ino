/* D1 Mini mit DHT11 Temperatur und Feuchte Sensor als
 *  Slave in einer ESP-NOW Kommunikation
 *  Der Slave erzeugt das WiFi Netz Slave_1 und wartet auf Daten vom Master
 *  Immer wenn ein Datenpaket empfangen wurde antwortet der Slave mit
 *  den Messwerten vom Sensor
 */

 //Bibliotheken für WiFi und für den Sensor
#include <esp_now.h>
#include <WiFi.h>

//WiFi Kanal
#define CHANNEL 1

//Datenstruktur für den Datenaustausch
struct DATEN_STRUKTUR {
    uint16_t temperatur = 0;
    uint32_t feuchte = 0;
};

//Instanz zum Zugriff auf den Sensor

//Datenpin am D1Mini = D0 entspricht GPIO16 
const byte dataPin = 16; //Daten pin des DHT11

//globale Variablen für Messwerte
byte temperatur = 0;
byte feuchte = 0;

//Wir brauchen einen WiFi Access Point
void configDeviceAP() {
  char* SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

//Callback funktion wenn Daten empfangen wurden 
void on_receive_data(uint8_t *mac, uint8_t *r_data, uint8_t len) {
    //zur Information wandeln wir die MAC Adresse des Absenders in einen String und geben sie aus
    char MACmaster[6];
    sprintf(MACmaster, "%02X:%02X:%02X:%02X:%02X:%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    Serial.print("Empfang von MAC: "); Serial.print(MACmaster);
 
    DATEN_STRUKTUR ed;
    //wir kopieren die empfangenen Daten auf die Datenstruktur um
    //über die Datenstruktur zugreifen zu können
    memcpy(&ed, r_data, sizeof(ed));

    //hier könnten wir über ed.temperatur und ed.feuchte auf jene Daten zugreifen
    //die der Absender geschickt hat
 
    //nun setzen wir die Werte mit den Daten vom Sensor
    ed.temperatur = 10;
    ed.feuchte = 20;

    //wir kopieren die daten in einen Speicherblock und senden sie zurück an den Absender
    uint8_t s_data[sizeof(ed)]; memcpy(s_data, &ed, sizeof(ed));
    esp_now_send(mac, s_data, sizeof(s_data));
  };
 

void setup() {
  Serial.begin(115200); Serial.println();
  //Konfiguration des Access Points
  WiFi.mode(WIFI_AP);
  configDeviceAP(); 
  //ESOP-Now initialisieren  
  if (esp_now_init()!=0) {
    Serial.println("Protokoll ESP-NOW nicht initialisiert...");
    ESP.restart();
    delay(1);
  }
 
  //***MAC Adressenb zur Info ***//
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  Serial.print("STA MAC: "); Serial.println(WiFi.macAddress());
 
  // ESP Rolle festlegen 1=Master, 2 = Slave 3 = Master + Slave
  esp_now_set_self_role(2); 
  //und callback funktion registrieren
  esp_now_register_recv_cb(on_receive_data);  
 
}

void loop() {
  // warte 1.5 s Der DHT11 macht 1 Messung / s
  delay(1500);
  
}

