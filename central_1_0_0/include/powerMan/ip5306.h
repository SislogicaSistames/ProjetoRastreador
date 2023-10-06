#ifndef _IP5306_H_
#define _IP5306_H_



#define I2C_SDA 21
#define I2C_SCL 22

#define IP5306_ADDR 0x75
#define IP5306_REG_SYS_CTL0 0x00



// I2C for SIM800 (to keep it running when powered from battery)


void init_i2c_serial();



#endif