#include <Arduino.h>

#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

/********************  MACROS PROPRIAS  *******************************/
#define INTERVAL_ENVIO            10000 // Milisegundos
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const char  buffer_espnow_string[] = "A|123|01|1";
// char espnow_buffer[50];
// snprintf(espnow_buffer, sizeof(espnow_buffer), "|%s", &buffer_espnow_string);


// Structure example to send data
// Must match the receiver structure
typedef struct etanol_espnow_send_t {
  int id;         // 
  char modulo[32];     // Nome do modulo
  double leitura;
  int alarme;
  char espnow_buf[50];

} etanol_espnow_send_t;

// Create a struct_message called myData
etanol_espnow_send_t data;

esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.print("buffer_espnow_string:\t"); Serial.println(buffer_espnow_string);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void envia_espnow()
{
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &buffer_espnow_string, sizeof(buffer_espnow_string));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(INTERVAL_ENVIO);

}


void espnow_start()
{
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

}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  
  espnow_start();
  
 
}
 
void loop() {


  envia_espnow();
}