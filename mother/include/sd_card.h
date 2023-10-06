#ifndef _SD_CARD_H
#define _SD_CARD_H

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define CS 15
#define SCK 14
#define MOSI 13
#define MISO 2



void escreve_arquivo(char* buffer);
void readFile(fs::FS &fs, const char * path);

void sd_init();







#endif