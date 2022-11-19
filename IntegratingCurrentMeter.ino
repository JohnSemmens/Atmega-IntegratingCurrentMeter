/*
    Name:       IntegratingCurrentMeter.ino
    Created:	10/06/2021 5:50:10 PM
    Author:     ACS\john
*/


#include "Adafruit_INA219.h"
#include <Wire.h>

Adafruit_INA219 ina219_1; // 0x40
Adafruit_INA219 ina219_2(0x41);
Adafruit_INA219 ina219_3(0x44);
Adafruit_INA219 ina219_4(0x45);

byte VI_1;
byte VI_2;
byte VI_3;
byte VI_4;

float PowerSensorV_1, PowerSensorI_1;
float PowerSensorV_2, PowerSensorI_2;
float PowerSensorV_3, PowerSensorI_3;
float PowerSensorV_4, PowerSensorI_4;


float PowerSensorV_2_min, PowerSensorV_2_max, PowerSensorV_2_sum, PowerSensorV_2_avg;
float PowerSensorI_2_min, PowerSensorI_2_max, PowerSensorI_2_sum, PowerSensorI_2_avg;

int IntegralCounter;
long EndTime;



//const 

void setup()
{
    Serial.begin(9600);  // start serial for output
    Serial.println("\nVI Logger V1.1 32V-2A");

    byte error, address;

    Wire.beginTransmission(0x40);
    VI_1 = Wire.endTransmission();

    Wire.beginTransmission(0x41);
    VI_2 = Wire.endTransmission();

    Wire.beginTransmission(0x44);
    VI_3 = Wire.endTransmission();

    Wire.beginTransmission(0x45);
    VI_4 = Wire.endTransmission();

    if (VI_1 == 0)
    {
        ina219_1.begin(); // I2C Address 0x40
        //ina219_1.setCalibration_16V_400mA();
        ina219_1.setCalibration_32V_2A();
        Serial.println("ina219 #1 found at 0x40");
    }

    if (VI_2 == 0)
    {
        ina219_2.begin(); // I2C Address 0x41
       // ina219_2.setCalibration_16V_400mA();
        ina219_2.setCalibration_32V_2A();
        Serial.println("ina219 #2 found at 0x41");
    }

    if (VI_3 == 0)
    {
        ina219_3.begin(); // I2C Address 0x44
       // ina219_3.setCalibration_16V_400mA();
        ina219_3.setCalibration_32V_2A();
        Serial.println("ina219 #3 found at 0x44");
    }

    if (VI_4 == 0)
    {
        ina219_4.begin(); // I2C Address 0x45
       // ina219_4.setCalibration_16V_400mA();
        ina219_4.setCalibration_32V_2A();
        Serial.println("ina219 #4 found at 0x45");
    }

}

void loop()
{
    if (millis() > EndTime)
    {
        // integration completed.
        // print results and reset.

        EndTime = millis() + 10000; // move out the end time

        PowerSensorV_2_avg = PowerSensorV_2_sum / IntegralCounter;
        PowerSensorI_2_avg = PowerSensorI_2_sum / IntegralCounter;

        Serial.print("samples:");
        Serial.print(IntegralCounter);
        Serial.print(" Vmin:");
        Serial.print(PowerSensorV_2_min);
        Serial.print(" Vmax:");
        Serial.print(PowerSensorV_2_max);
        Serial.print(" Vavg:");
        Serial.print(PowerSensorV_2_avg);
        Serial.print(" Imin:");
        Serial.print(PowerSensorI_2_min);
        Serial.print(" Imax:");
        Serial.print(PowerSensorI_2_max);
        Serial.print(" Iavg:");
        Serial.print(PowerSensorI_2_avg);
        Serial.println();


        // reset 
        IntegralCounter = 0;
        PowerSensorV_2_sum = 0;
        PowerSensorV_2_min = 99;
        PowerSensorV_2_max = -99;
        PowerSensorI_2_sum = 0;
        PowerSensorI_2_min = 99;
        PowerSensorI_2_max = -99;
    }
    else
    {
        // normal integration step.
        IntegralCounter++;

        if (VI_2 == 0)
        {
            PowerSensorV_2 = ina219_2.getBusVoltage_V();
            PowerSensorI_2 = ina219_2.getCurrent_mA();

            if (PowerSensorV_2_min > PowerSensorV_2) PowerSensorV_2_min = PowerSensorV_2;
            if (PowerSensorV_2_max < PowerSensorV_2) PowerSensorV_2_max = PowerSensorV_2;        
            PowerSensorV_2_sum = PowerSensorV_2_sum + PowerSensorV_2;

            if (PowerSensorI_2_min > PowerSensorI_2) PowerSensorI_2_min = PowerSensorI_2;
            if (PowerSensorI_2_max < PowerSensorI_2) PowerSensorI_2_max = PowerSensorI_2;
            PowerSensorI_2_sum = PowerSensorI_2_sum + PowerSensorI_2;
        }
  
    }








}
