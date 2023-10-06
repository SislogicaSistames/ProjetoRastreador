#ifndef     _GPS_H_
#define     _GPS_H_

#include <TinyGPSPlus.h>
#include "common.h"


struct gps_info
{
    int id;
    double lat;
    double lng;
    int numSat;
    float hdop;
    float altitude;
    float course;
    float speed;
        
};
typedef struct gps_info gps_t;
   


// GPS pins
static const int RXPin = 25, TXPin = 3;
static const uint32_t GPSBaud = 9600;

extern char buffer[100];

void gps_init();





#endif