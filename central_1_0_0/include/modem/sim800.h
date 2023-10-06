#ifndef _SIM800_H_
#define _SIM800_H_

#include <Arduino.h>



// Definido antes da inclusao da biblioteca
#define TINY_GSM_MODEM_SIM800  
#include "TinyGSM.h"
#include <TinyGsmClient.h>
#include "espnow/espnow.h"
#include "gps/gps.h"



// TTGO T-Call pins for SIM800L
#define MODEM_RST 5
#define MODEM_PWKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 27
#define MODEM_RX 26



// set GSM PIN, if any
#define GSM_PIN ""

extern recv_espnow_data_t data;
extern gps_t *pGPS;

void modem_init();
void mqtt_init();

void conecta_mqtt();
void envia_mqtt();

#endif