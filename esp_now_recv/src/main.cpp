#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct etanol_espnow_send_t {
  int id;         // 
  char modulo[32];     // Nome do modulo
  double leitura;

} etanol_espnow_send_t;

// Create a struct_message called myData
etanol_espnow_send_t data;


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data.modulo, incomingData, sizeof(data.modulo));
  
  Serial.println(data.modulo);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else{
    Serial.println("Initialized");
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}