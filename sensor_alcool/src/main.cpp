#include <Arduino.h>


//Include the library
#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("ESP32")
#define         Pin                     (34)  //Analog input 34 of your ESP32
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-3") //MQ3
#define         Voltage_Resolution      (3)
#define         ADC_Bit_Resolution      (12) // For arduino UNO/MEGA/NANO
#define         RatioMQ3CleanAir        (60) //RS / R0 = 60 ppm 

/*****************************Globals***********************************************/
double          alcoholPPM          =   (0);
/**************************Object_Sensor********************************************/
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(115200); //Init serial port

  //Set math model to calculate the PPM concentration and the value of constants
  MQ3.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ3.setA(0.3934); MQ3.setB(-1.504); //Configure the equation to calculate Alcohol concentration value


  /*
    Exponential regression:
  Gas    | a      | b     
  LPG    | 44771  | -3.245
  CH4    | 2*10^31| 19.01
  CO     | 521853 | -3.821
  Alcohol| 0.3934 | -1.504
  Benzene| 4.8387 | -2.68
  Hexane | 7585.3 | -2.849
  */

  
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ3.setRL(1);
  
  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ3.init();
  // /*****************************  MQ CAlibration ********************************************/ 
  // // Explanation: 
  //  // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
  // // and on clean air (Calibration conditions), setting up R0 value.
  // // We recomend executing this routine only on setup in laboratory conditions.
  // // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
  // // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  // Serial.print("Calibrating please wait.");
  // float calcR0 = 0;
  // for(int i = 1; i<=10; i ++)
  // {
  //   MQ3.update(); // Update data, the arduino will read the voltage from the analog pin
  //   calcR0 += MQ3.calibrate(RatioMQ3CleanAir);
  //   Serial.print(".");
  // }
  // MQ3.setR0(calcR0/10);
  // Serial.println("  done!.");
  // Serial.println(calcR0/10);

  // delay(3000);
  
  // if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  // if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 

  MQ3.setR0(0.03);

}

void loop() {
  MQ3.update(); // Update data, the arduino will read the voltage from the analog pin
  alcoholPPM = MQ3.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
  Serial.print("Alcohol now (PPM): ");
  Serial.println(alcoholPPM);
  delay(500); //Sampling frequency
}