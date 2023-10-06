#include "sd_card.h"


SPIClass spi = SPIClass(HSPI);

void writeFile(fs::FS &fs, const char * path, const char * message){

    spi.begin(SCK, MISO, MOSI, CS);
    

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

    spi.end();
}

void appendFile(fs::FS &fs, const char * path, const char * message){

    if(!SD.begin(CS, spi)){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();

    SD.end();

    
}

void readFile(fs::FS &fs, const char * path){

    spi.begin(SCK, MISO, MOSI, CS);
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

    spi.end();
}

void escreve_arquivo(char* buffer){
    appendFile(SD,"/offline_readings.txt", buffer);
}

void sd_init(){
    spi.begin(SCK, MISO, MOSI, CS);
    
}