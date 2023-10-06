
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "common.h"

#define CS 15
#define SCK 14
#define MOSI 13
#define MISO 2

void sd_init();
void writeFile(fs::FS &fs, const char * path, const char * message);
void readFile(fs::FS &fs, const char * path);

void escreve_arq(const char* menssagem);
void le_arq();