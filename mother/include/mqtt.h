#ifndef _MQTT_SERV_H
#define _MQTT_SERV_H

#include <PubSubClient.h>
#include "common.h"
#include "sim800.h"


extern TinyGsm modem;

#define MQTT_BROKER         "tmslog.com.br"
#define TOPIC_MQTT          "central/"
#define TOPIC_MQTT_ACC          "ACC/"

/**
 * mqtt.state() retorna os seguitnes valores, porem está definido como macros e dentro da biblioteca.
 * Os estatos serao passados para enumeradores para facilitar entendimento
 * @note -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
         -3 : MQTT_CONNECTION_LOST - the network connection was broken
         -2 : MQTT_CONNECT_FAILED - the network connection failed
         -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
          0 : MQTT_CONNECTED - the client is connected
          1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
          2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
          3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
          4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
          5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
*/

typedef enum mqtt_message_e{
    MQTT_CONEXAO_TIMEOUT = MQTT_CONNECTION_TIMEOUT,
    MQTT_CONEXAO_PERDIDA = MQTT_CONNECTION_LOST,
    MQTT_CONEXAO_FALHOU = MQTT_CONNECT_FAILED,
    MQTT_DESCONECTADO = MQTT_DISCONNECTED,
    MQTT_CONECTADO = MQTT_CONNECTED,
    MQTT_CONEXAO_BAD_PROTOCOL = MQTT_CONNECT_BAD_PROTOCOL,
    MQTT_CONEXAO_BAD_CLIENT_ID = MQTT_CONNECT_BAD_CLIENT_ID,
    MQTT_CONEXAO_INDISPONIVEL = MQTT_CONNECT_UNAVAILABLE,
    MQTT_CONEXAO_BAD_CREDENTIALS = MQTT_CONNECT_BAD_CREDENTIALS,
    MQTT_CONEXAO_NAO_AUTORIZADA = MQTT_CONNECT_UNAUTHORIZED,
}mqtt_message_e;

typedef struct mqtt_message_t{
    mqtt_message_e msgID;
}mqtt_message_t;

void mqtt_init();
void mqtt_online_send(char* buffer);
void write_sd_card(char* buffer);

BaseType_t mqtt_send_state_message(mqtt_message_e msgID);







#endif