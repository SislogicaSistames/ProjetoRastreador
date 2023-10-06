#include <Arduino.h>

#include "gps.h"
#include "mqtt.h"
#include "common.h"


TinyGPSPlus gps;


gps_t *pGPS =  (gps_t *)malloc(sizeof(gps_t));

char buffer_gps[100];


static void leitura_gps(void *vParameters)
{
    while(1)
    {    
        vTaskDelay(5000/portTICK_PERIOD_MS);

        // pGPS->id = 4322;
        // pGPS->lat = gps.location.lat();
        // pGPS->lng = gps.location.lng();
        // pGPS->numSat = gps.satellites.value();
        // pGPS->hdop = gps.hdop.hdop();
        // pGPS->altitude = gps.altitude.meters();
        // pGPS->course = gps.course.deg();
        // pGPS->speed = gps.speed.kmph();

        pGPS->id = 4322;
        pGPS->lat = 36.5;
        pGPS->lng = -23.5;
        pGPS->numSat = 11;
        pGPS->hdop = 2;
        pGPS->altitude = 600;
        pGPS->course = 5;
        pGPS->speed = 56;



        // if(g_mqtt_status == MQTT_CONECTADO && gps.location.isUpdated() && gps.location.isValid()){
        //     snprintf(buffer_gps, sizeof(gps_t)+3, "%d;GPS;%.2f;%.2f;%d;%.2f;%.2f;%.2f;%.2f;", pGPS->id, pGPS->lat, pGPS->lng,pGPS->numSat,pGPS->hdop,pGPS->altitude,pGPS->course,pGPS->speed);

        //     mqtt_online_send(buffer_gps);
        // }

        if(g_mqtt_status == MQTT_CONECTADO){
            snprintf(buffer_gps, sizeof(gps_t)+3, "%d;GPS;%.2f;%.2f;%d;%.2f;%.2f;%.2f;%.2f;", pGPS->id, pGPS->lat, pGPS->lng,pGPS->numSat,pGPS->hdop,pGPS->altitude,pGPS->course,pGPS->speed);

            mqtt_online_send(buffer_gps);
        }
        else{
            // write_sd_card(buffer_gps);
            // Serial.println(buffer_gps);
        }
    }
}
void gps_init()
{
     SerialGPS.begin(9600, SERIAL_8N1, RXPin, TXPin);
     
    // xTaskCreatePinnedToCore(leitura_gps, "leitura_gps", 2048, NULL, 1,; NULL, 0);
}
