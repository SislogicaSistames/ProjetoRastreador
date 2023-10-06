#include <Arduino.h>
#include "sd/sd_card.h"


SPIClass spi = SPIClass(HSPI);

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void sd_init(){
    spi.begin(SCK, MISO, MOSI, CS);
    delay(10);

    if(!SD.begin(CS,spi)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
}

void escreve_arq(const char* mensagem)
{
    writeFile(SD, "/offline_readings.txt", mensagem);
}

void le_arq()
{
    
}

