#ifndef _SIM800_H_
#define _SIM800_H_

#include <Arduino.h>



// Definido antes da inclusao da biblioteca
#define TINY_GSM_MODEM_SIM800  
#include "TinyGSM.h"
#include "TinyGsmClient.h"
#include "common.h"


// TTGO T-Call pins for SIM800L
#define MODEM_RST               5
#define MODEM_PWKEY             4
#define MODEM_POWER_ON          23
#define MODEM_TX                27
#define MODEM_RX                26

// set GSM PIN, if any
#define GSM_PIN ""

#define APN "sislogica.algar.br"
#define GPRS_USER "algar"
#define GPRS_PWD "1212"

extern char buffer_gps[100];

typedef enum modem_status_e{
    MODEM_DESCONECTADO = 0,
    MODEM_CONECTADO,
    MODEM_CONECTANDO,
}modem_status_e;

typedef enum modem_message_e{
    GPRS_CONECTADO = 0,
    GPRS_CONECTANDO,
    GPRS_DESCONECTADO,
}modem_message_e;

typedef struct modem_message_t{
    modem_message_e msgID;
}modem_message_t;

void modem_init();


#endif