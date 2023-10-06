#include <Arduino.h>

#include "common.h"
#include "modem/sim800.h"
#include "powerMan/ip5306.h"
#include "mqtt/mqtt.h"
#include "sd/sd_card.h"



TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

PubSubClient mqtt(client);


boolean status;



// Your GPRS credentials, if any
const char apn[] = "sislogica.algar.br";  // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = "algar";
const char gprsPass[] = "1212";

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";

// MQTT broker e topic
const char* topic = TOPIC_MQTT;
const char* topicAcc = TOPIC_MQTT_ACC;
const char* broker = MQTT_BROKER;

// Buffer para recebimento de espnow. Salva dados recebidos
char buffer[100];


static void sim800_info(){
    SerialMon.println("Initializing modem...");
    modem.restart();

    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);

    if (GSM_PIN && modem.getSimStatus() != 3) {
        modem.simUnlock(GSM_PIN);
    }

    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        SerialMon.println(" fail");
        ESP.restart();
    } else {
        SerialMon.println(" OK");
    }

    if (modem.isGprsConnected()) {
        SerialMon.println("GPRS connected");
  }
}

void modem_init()
{
    init_i2c_serial();

    SerialMon.begin(115200);
    delay(10);     // Monitor serial via UART

    // SerialGPS.begin(9600, SERIAL_8N1, RXPin, TXPin);
    // delay(10);
    
    pinMode(MODEM_PWKEY, OUTPUT);
    pinMode(MODEM_RST, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);
    digitalWrite(MODEM_PWKEY, LOW);
    digitalWrite(MODEM_RST, HIGH);
    digitalWrite(MODEM_POWER_ON, HIGH);

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(30);

    sim800_info();

}

/**
 * Sessão MQTT -- Não consegui instanciar PubSubClient mqtt(client) em outro arquivo ou passar como parametro
 * @note A biblioteca tem como objetivo a Arduino IDE, que implementa diversas coisas por baixo dos panos
*/

static boolean mqttConnect() {
  SerialMon.print("Connecting to ");
  SerialMon.print(broker);

  // Connect to MQTT Broker without username and password
  status = mqtt.connect("432567896543");

  // Or, if you want to authenticate MQTT:
  // boolean status = mqtt.connect("GsmClientN", mqttUsername, mqttPassword);

  if (status == false) {
    SerialMon.println(" fail");
    // ESP.restart();
    return false;
  }
  SerialMon.println(" success");
  // mqtt.subscribe(topic);
  // mqtt.subscribe(topicAcc);


  return mqtt.connected();
}

static void mqttCallback(char* topic, byte* message, unsigned int len) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < len; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // if statements para validação dos topicos
}

static void mqtt_broker_setup()
{
  mqtt.setServer(broker, 51510);
  mqtt.setCallback(mqttCallback);
}
void mqtt_init()
{
    mqtt_broker_setup();
}

void task_conec_mqtt(void *vParameter)
{
  while(1)
  {  
    // loop() =  This should be called regularly to allow the client to process incoming messages and maintain its connection to the server.
    mqtt.loop();
    SerialMon.println(mqtt.state());

    bool mqtt_conectado = mqtt.connected();

    vTaskDelay(10000/ portTICK_PERIOD_MS); // Retirar isso da task por conta do watchdog

    if(!mqtt_conectado) {
        // Reconnect every 10 seconds
        
          if (mqttConnect()) {
            Serial.println("Conectado");
            // break;
          }
      }
  }
}

void conecta_mqtt()
{
  mqttConnect();
  // vTaskDelay(10/portTICK_RATE_MS);
  xTaskCreate(task_conec_mqtt, "Conecta_mqtt", 2048, NULL, 1, NULL);
}

/**
 * Caso o modulo esteja alarmado, deve enviar um MQTT no mesmo momento
 * 
*/
void task_envia_mqtt(void *vParameters)
{
  while(1)
  {
    if(data.alarme == 1 || strlen(data.recv_buf)>0 && status == true)
    {
      escreve_arq(data.recv_buf);
      mqtt.publish(topicAcc, data.recv_buf);
      bzero(data.recv_buf, 100);
      le_arq();
    }
    

    vTaskDelay(5000/ portTICK_PERIOD_MS);

    mqtt.publish(topic, buffer);

  }

}

void envia_mqtt()
{
  xTaskCreate(task_envia_mqtt, "envia_mqtt", 2048, NULL, 1, NULL);
}



