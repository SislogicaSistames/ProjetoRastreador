#include <Arduino.h>

#include "sim800.h"
#include "common.h"
#include "mqtt.h"
#include "rgb_led.h"
#include "freertos/task.h"

#define GENERATE_RUN_TIME_STATS 1
#define USE_STATS_FORMATTING_FUNCTIONS 1
TinyGsm modem(SerialAT);

QueueHandle_t modem_queue_handle;

int g_modem_status = MODEM_DESCONECTADO;


static BaseType_t modem_send_message(modem_message_e msgID)
{
    modem_message_t msg;
    msg.msgID = msgID;

    return xQueueSend(modem_queue_handle, &msg, portMAX_DELAY);
}

static void  gprs_connected(){

    if(!modem.isGprsConnected()){
        // printf("OI");

        if (!modem.gprsConnect(APN, GPRS_USER, GPRS_PWD)) {
            SerialMon.println(" fail    1234");
            // ESP.restart();
            
            
            modem_send_message(GPRS_DESCONECTADO);
            
        } else {
            SerialMon.print(" OK  ");
            SerialMon.println("GPRS connected");
            
            modem_send_message(GPRS_CONECTADO);

        }
    }
    
}

static void sim800_info(){

    SerialMon.println("Initializing modem...");
    modem.restart();

    String modemInfo = modem.getModemInfo();
    SerialMon.print("Modem Info: ");
    SerialMon.println(modemInfo);

}

static void unlock_sim800()
{
    if (GSM_PIN && modem.getSimStatus() != 3) {
        modem.simUnlock(GSM_PIN);
        SerialMon.println("                 Unlocked");
    }
}

static void modem_task(void *pvParameters)
{
    modem_message_t msg;
    char buffer[45];

    for(;;){
        if(!modem.isGprsConnected()){

        //     modem_send_message(GPRS_DESCONECTADO);
        // }

        if(xQueueReceive(modem_queue_handle, &msg, portMAX_DELAY)){
            switch(msg.msgID)
            {
                case GPRS_DESCONECTADO:
                    SerialMon.println("GPRS_DESCONECTADO");

                    // led_gprs_desconectado();
                    g_modem_status = MODEM_DESCONECTADO;

                    modem_send_message(GPRS_CONECTANDO);
                    
                    break;

                case GPRS_CONECTANDO:
                    SerialMon.println("GPRS_CONECTANDO");

                    g_modem_status = MODEM_CONECTANDO;

                    // funcao fornece mais informações da task, porem nao foi compilado na Arduino Framework
                    // vTaskGetRunTimeStats(buffer);

                    // led_gprs_conectando();
                    gprs_connected();
                    
                    break;

                case GPRS_CONECTADO:
                    SerialMon.println("GPRS_CONECTADO");

                    g_modem_status = MODEM_CONECTADO;
                    // led_gprs_conectado();

                    mqtt_send_state_message(MQTT_DESCONECTADO);
                    //acende led
                    break;
                default:
                    break;
            }
        }
    }
}
void modem_init()
{

    pinMode(MODEM_PWKEY, OUTPUT);
    pinMode(MODEM_RST, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);
    digitalWrite(MODEM_PWKEY, LOW);
    digitalWrite(MODEM_RST, HIGH);
    digitalWrite(MODEM_POWER_ON, HIGH);

    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(30);

    unlock_sim800();

    modem_queue_handle = xQueueCreate(3,sizeof(modem_message_t));
    
    xTaskCreatePinnedToCore(modem_task, "modem_task", 2048, NULL, 4, NULL, 1);

    modem_send_message(GPRS_DESCONECTADO);

}