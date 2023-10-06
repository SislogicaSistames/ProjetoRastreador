#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("ESP32")
#define         Pin                     (34)  //Analog input 34 of your ESP32
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-3") //MQ3
#define         Voltage_Resolution      (3.3)
#define         ADC_Bit_Resolution      (12) // For arduino UNO/MEGA/NANO
#define         RatioMQ3CleanAir        (60) //RS / R0 = 60 ppm 


/********************  MACROS PROPRIAS  *******************************/
#define ID_MODULO                       261204
#define NOME_MODULO                     "Etilometro"
#define INTERVAL_ENVIO                  8000            // Em milisegundos
#define PPM_MAX_ALARME                  7.0

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

double          alcoholPPM          =   (0);
double          alcoholPPMUltimo    =   (0);

char buffer_espnow_string[50];

int intervalo = 60000;
int intervaloalarm = 10000;
int inter = 1000000;
int varicao = 20;
int tpvar = 1;
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);


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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(alcoholPPM);
  Serial.println(buffer_espnow_string);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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

void MQ3_init()
{
   // Configure MQ-3
  MQ3.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ3.setA(0.3934); MQ3.setB(-1.504); //Configure the equation to calculate Alcohol concentration value

  MQ3.setRL(1);           // RL - Resistor de que controla a tensao de saída
  MQ3.setR0(16.6);
}


int ver_alarme(int alcoholPPM)
{
  if(alcoholPPM > PPM_MAX_ALARME)
  {
    return 1;
  }
  
  return 0;
  
};
void envia_espnow(int var,int eta)
{
  // Send message via ESP-NOW
   
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data.espnow_buf, sizeof(buffer_espnow_string));
   
  if (result == ESP_OK) {
    Serial.print("Sent with success \t");
    Serial.print(var);
    Serial.print("-");
    Serial.print(eta);
    Serial.println(var);
  }
  else {
    Serial.print("Error sending the data \t");
    Serial.println(var);
  }
 
}
void prep_envio()
{
  // Set values to send
  strcpy(data.modulo, NOME_MODULO);
  data.id = ID_MODULO;
  MQ3.update();
  alcoholPPM = MQ3.readSensor();
  // data.leitura = alcoholPPM;
    snprintf(data.espnow_buf , sizeof(data.espnow_buf), "%s|%d|%.2f|%d",data.modulo,data.id,alcoholPPM, data.alarme);
  if(ver_alarme(alcoholPPM)&&data.alarme == 0)
  {
     envia_espnow(1,alcoholPPM);
     alcoholPPMUltimo =alcoholPPM;
    data.alarme = 1;
    
  }
  if(!ver_alarme(alcoholPPM)&&data.alarme == 1)
   {
    envia_espnow(10,alcoholPPM);
    alcoholPPMUltimo =alcoholPPM;
    data.alarme = 0;
   
  }

if(inter >= (data.alarme == 0?intervalo:intervaloalarm)){
  envia_espnow(2,alcoholPPM);
  alcoholPPMUltimo = alcoholPPM;
  inter = 0;
}
if(tpvar=1){
if(alcoholPPMUltimo > alcoholPPM+(alcoholPPM*varicao/100)||alcoholPPMUltimo < alcoholPPM-(alcoholPPM*varicao/100))
{
  envia_espnow(3,alcoholPPM);
  alcoholPPMUltimo =alcoholPPM;
}}
else
{
  if(alcoholPPMUltimo > alcoholPPM+varicao||alcoholPPMUltimo < alcoholPPM-varicao)
{
  envia_espnow(4,alcoholPPM);
  alcoholPPMUltimo =alcoholPPM;
}
}


}

    // Serial.println(sizeof(data.espnow_buf));
    // Serial.println();
    // Serial.println(strlen(data.espnow_buf));

 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  
  espnow_start();
  
  MQ3_init();
 
}
 
void loop() {
 
  prep_envio();
  
  Serial.println(data.espnow_buf);

 
   vTaskDelay(1000/portTICK_PERIOD_MS);
   inter = inter + 1000;
}