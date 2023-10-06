#include <Arduino.h>
#include "espnow/espnow.h"
#include <esp_now.h>
#include <WiFi.h>

recv_espnow_data_t data;

static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));

  if (data.alarme == 1)
  {
    Serial.println("Alarmado!!!!");
  }

  
  

}

void espnow_init(){

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