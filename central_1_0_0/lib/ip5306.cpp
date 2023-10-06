#include "powerMan/ip5306.h"
#include <Wire.h>
#include "common.h"

TwoWire I2CPower = TwoWire(0);

static bool setPowerBoostKeepOn(int en) {
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37);  // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35);  // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

void init_i2c_serial()
{
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);
  bool isOk = setPowerBoostKeepOn(1);

  // Pode indicar com LED, ou ate mesmo o proprio modem estar ligado
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

}