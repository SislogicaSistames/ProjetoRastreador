#include <Arduino.h>
#include "gps/gps.h"


TinyGPSPlus gps;


gps_t *pGPS =  (gps_t *)malloc(sizeof(gps_t));


static void leitura_gps(void *vParameters)
{
    while(1)
    {    vTaskDelay(3000/portTICK_PERIOD_MS);
        snprintf(buffer, sizeof(gps_t)+3, "%d;GPS;%.2f;%.2f;%d;%.2f;%.2f;%.2f;%.2f;", pGPS->id, pGPS->lat, pGPS->lng,pGPS->numSat,pGPS->hdop,pGPS->altitude,pGPS->course,pGPS->speed);

        pGPS->id = 4322;
        pGPS->lat = gps.location.lat();
        pGPS->lng = gps.location.lng();
        pGPS->numSat = gps.satellites.value();
        pGPS->hdop = gps.hdop.hdop();
        pGPS->altitude = gps.altitude.meters();
        pGPS->course = gps.course.deg();
        pGPS->speed = gps.speed.kmph();
    }
}
void gps_init()
{
     SerialGPS.begin(9600, SERIAL_8N1, RXPin, TXPin);
     
    xTaskCreate(leitura_gps, "leitura_gps", 2048, NULL, 1, NULL);
}
