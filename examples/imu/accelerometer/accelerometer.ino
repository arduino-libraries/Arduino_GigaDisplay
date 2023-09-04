/*
  Arduino BMI270 - Simple Accelerometer

  This example reads the acceleration values from the BMI270
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - GIGA R1 WiFi
  - GIGA Display Shield

  created 10 Jul 2019
  by Riccardo Rizzo

  modified 4 sept 2023
  by Karl Söderby

  Note: this example originates from the following library:
  https://github.com/arduino-libraries/Arduino_BMI270_BMM150

  This example code is in the public domain.
*/

#include "Arduino_BMI270_BMM150.h"
BoschSensorClass imu(Wire1);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!imu.begin()) {
    Serial.println("Failed to initialize imu!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(imu.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}

void loop() {
  float x, y, z;

  if (imu.accelerationAvailable()) {
    imu.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }
}