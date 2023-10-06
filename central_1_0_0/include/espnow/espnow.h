#ifndef     _ESPNOW_H_
#define     _ESPNOW_H_

#include <Arduino.h>


typedef struct message_espnow_recv_t {
    char recv_buf[100];
    int alarme;
} recv_espnow_data_t;

// extern char buffer_mqtt[100];





void espnow_init();


#endif