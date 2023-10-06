#include <Arduino.h>
#include <SoftwareSerial.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct espnow_recv_t{
    char buffer[50];
}espnow_recv_t;

espnow_recv_t data;

esp_now_peer_info_t peerInfo;

String readString;
String tipo_comando;
String interacao;

int intervalo = 10000;

// const char  buffer_espnow_string[5];

char comando;


// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data.buffer, incomingData, sizeof(data));
 
  Serial2.println((char*)data.buffer);
  Serial.println((char*)data.buffer);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.print("buffer_espnow_string:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  readString.clear();
}

void envia_espnow()
{
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &readString, sizeof(readString));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    readString.clear();
  }
  else {
    Serial.println("Error sending the data");
  }

}

void setup() {
  Serial2.begin(9600, SERIAL_8N1, 18,19);
  Serial.begin(115200);

    // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);


  if (esp_now_init() != ESP_OK) {
    Serial2.println("Error initializing ESP-NOW");
    return;
  }
  else{
    Serial2.println("Initialized");
  }
  

  esp_now_register_recv_cb(OnDataRecv);
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

void loop() {
  // vTaskDelay(500/portTICK_PERIOD_MS);
  // Serial2.println("Mensagem      *****");
  
  if(Serial2.available())
  {
    comando = Serial2.read();

    
    if(comando == '\0')
    {
      // envia_espnow();
      // readString.clear();
      

      Serial.println("Envia_espnow");
      Serial.flush();
      // Serial.println(comando);
    }
    
    else{
      readString += comando;
      // Serial.println(comando);
      Serial.println(readString);
    }

    

    // Serial.print(comando);
  }

  if(readString == "E|PING")
    {
      envia_espnow();
      
      Serial.flush();
      readString = "";
      

      // Serial.println("Envia_espnow");
      // Serial.println(comando);
    }
  
  if(readString == "E|2PING")
    {
      // envia_espnow();
      Serial.println("Chegou mensagem");
      Serial2.println("Chegou mensagem");
      
      Serial.flush();
      readString = "";
      

      // Serial.println("Envia_espnow");
      // Serial.println(comando);
    }
  
  
  

  
}
