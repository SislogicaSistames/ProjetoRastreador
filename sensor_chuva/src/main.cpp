#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// DEfine pino de entrada do sensor de chuva 
#define SENSOR_CHUVA                    34
#define ID_MODULO                       261204
#define NOME_MODULO                     "Sensor_Chuva"
#define INTERVAL_ENVIO                  15000            // Em milisegundos



typedef struct etanol_espnow_send_t {
  int id;         // 
  char modulo[32];     // Nome do modulo
  double leitura;
  int alarme;
  char espnow_buf[50];

} etanol_espnow_send_t;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Create a struct_message called myData
etanol_espnow_send_t data;

esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void envia_espnow()
{
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data.espnow_buf, sizeof(data.espnow_buf));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  // delay(INTERVAL_ENVIO);
  vTaskDelay(INTERVAL_ENVIO/portTICK_PERIOD_MS);

}


void setup() {
  Serial.begin(115200);

  pinMode(SENSOR_CHUVA, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);


    // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  strcpy(data.modulo, NOME_MODULO);
  data.id = 123876;

}

void loop() {

  // 0 - Nao esta chovendo
  // 1 - Está chovendo
  if(!digitalRead(SENSOR_CHUVA))
  {
    snprintf(data.espnow_buf , sizeof(data.espnow_buf), "%s|%d|0",data.modulo,data.id);

    Serial.println("Nao esta chovendo");

    digitalWrite(2,LOW);

    envia_espnow();
  }
  else{
    snprintf(data.espnow_buf , sizeof(data.espnow_buf), "%s|%d|1",data.modulo,data.id);

    Serial.println("Esta chovendo");

    digitalWrite(2,HIGH);

    envia_espnow();
  }

}
