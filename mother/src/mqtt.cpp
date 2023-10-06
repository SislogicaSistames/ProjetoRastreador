#include <Arduino.h>

#include "mqtt.h"
#include "sd_card.h"
#include "rgb_led.h"



TinyGsmClient client(modem);
PubSubClient mqtt(client);

QueueHandle_t mqtt_queue_handle;

static EventGroupHandle_t mqtt_event_group;

const int MQTT_CONECTADO_BIT = BIT0;

int g_mqtt_status = MQTT_DESCONECTADO;


BaseType_t mqtt_send_state_message(mqtt_message_e msgID){
    mqtt_message_t msg;
    msg.msgID = msgID;

    Serial.println(msgID);

    return xQueueSend(mqtt_queue_handle, &msg, portMAX_DELAY);
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
}

static void mqtt_conect()
{
    vTaskDelay(5000);

    bool status;
    // vTaskDelay(100);
    
    SerialMon.print("Connecting to ");
    SerialMon.println(MQTT_BROKER);

    mqtt.connect("Client_Name_261204");

    // status = mqtt.connect("Client_Name_261204");

    mqtt_send_state_message(MQTT_CONECTADO);


    // if (status == false) {
    //     SerialMon.println(" fail");
    //     // vTaskDelay(500/portTICK_PERIOD_MS);
    // }
    // else{
    //     SerialMon.println(" success");
    //     // mqtt.subscribe(TOPIC_MQTT);
    //     // mqtt.subscribe(topicAcc);



    // }
}

static void mqtt_reconnect(){
    mqtt_conect();
}

void mqtt_loop_task(void *pvParameter){
    EventBits_t eventBits;

    for(;;){
        bool status = mqtt.loop();
        Serial.println(mqtt.state());
        if(!status && g_mqtt_status == MQTT_CONECTADO){
            
            mqtt_send_state_message(MQTT_DESCONECTADO);
        }

        
        // if(mqtt.state() != 0){
        //     mqtt_conect();
        //     // tentativas_reconecxao = tentativas_reconecxao + 1; 
        // }  

        // eventBits = xEventGroupGetBits(mqtt_event_group);

        // if(eventBits & MQTT_CONECTADO_BIT){
        //     mqtt.publish(TOPIC_MQTT, "Hello world from Event 'MQTT_CONECTADO'");

        // }
        
        vTaskDelay(4000/ portTICK_PERIOD_MS);
    }
}

void mqtt_online_send(char* buffer)
{
    mqtt.publish(TOPIC_MQTT, buffer);
}

// void write_sd_card(char* buffer){
    
//     escreve_arquivo(buffer);
//     Serial.println("escreveu no cartao");
// }

void mqtt_task( void *pvParameter){
    mqtt_message_t msg;

    for(;;){
        if(xQueueReceive(mqtt_queue_handle,&msg, portMAX_DELAY))
        {
            switch(msg.msgID){
                case MQTT_CONEXAO_TIMEOUT:
                    SerialMon.println("MQTT_CONNECTION_TIMEOUT");

                    g_mqtt_status = MQTT_DESCONECTADO;


                    
                    break;
                case MQTT_CONEXAO_PERDIDA:
                    SerialMon.println("MQTT_CONNECTION_LOST");

                    g_mqtt_status = MQTT_DESCONECTADO;


                    
                    break;
                case MQTT_CONEXAO_FALHOU:
                    SerialMon.println("MQTT_CONNECT_FAILED");

                    g_mqtt_status = MQTT_DESCONECTADO;


                 
                    break;
                case MQTT_DESCONECTADO:
                    SerialMon.println("MQTT_DISCONNECTED");

                    g_mqtt_status = MQTT_DESCONECTADO;

                    led_mqtt_desconectado();


                    if(g_modem_status == MODEM_CONECTADO){
                        mqtt_conect();
                    } 

      
                    break;
                case MQTT_CONECTADO:
                    SerialMon.println("MQTT_CONNECTED");

                    g_mqtt_status = MQTT_CONECTADO;    

                    led_mqtt_conectado();                

                    xTaskCreatePinnedToCore(mqtt_loop_task, "mqtt_loop_task", 2048, NULL, 1, NULL, 0);
                    

                    

                    break;
                case MQTT_CONEXAO_BAD_PROTOCOL:
                    SerialMon.println("MQTT_CONNECT_BAD_PROTOCOL");

                    g_mqtt_status = MQTT_DESCONECTADO;


         
                    break;
                case MQTT_CONEXAO_BAD_CLIENT_ID:
                    SerialMon.println("MQTT_CONNECT_BAD_CLIENT_ID");

                    g_mqtt_status = MQTT_DESCONECTADO;


                    
                    break;
                case MQTT_CONEXAO_INDISPONIVEL:
                    SerialMon.println("MQTT_CONNECT_UNAVAILABLE");
                    g_mqtt_status = MQTT_DESCONECTADO;


                    
                    break;
                case MQTT_CONEXAO_BAD_CREDENTIALS:
                    SerialMon.println("MQTT_CONNECT_BAD_CREDENTIALS");
                    g_mqtt_status = MQTT_DESCONECTADO;


                    
                    break;
                case MQTT_CONEXAO_NAO_AUTORIZADA:
                    SerialMon.println("MQTT_CONNECT_UNAUTHORIZED");
                    g_mqtt_status = MQTT_DESCONECTADO;


                    
                    break;

                default:
                    break;
            }
        }
    }
}



void mqtt_init(){
    mqtt.setServer(MQTT_BROKER, 51510);
    mqtt.setCallback(mqttCallback);

    mqtt_queue_handle = xQueueCreate(10,sizeof(mqtt_message_e));

    mqtt_event_group = xEventGroupCreate();

    xTaskCreatePinnedToCore(mqtt_task, "mqtt_task", 4096, NULL, 4, NULL, 0);
    
}