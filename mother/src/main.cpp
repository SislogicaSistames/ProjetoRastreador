#include <Arduino.h>

#include "ip5306.h"
#include "sim800.h"
#include "mqtt.h"
#include "common.h"
#include "rgb_led.h"
#include "gps.h"
#include "sd_card.h"



void setup() {
  SerialMon.begin(115200);

  led_init();

  power_modem();

  mqtt_init();

  modem_init();
  
  gps_init();

  // sd_init();


  
  
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}

