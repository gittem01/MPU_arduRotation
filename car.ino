#include "Wire.h"
#include <MPU6050_light.h>

#define enA 9
#define enB 10
#define in1 6
#define in2 7
#define in3 11
#define in4 12
 
MPU6050 mpu(Wire);
unsigned long timer = 0;
unsigned long myTimer = 0;

float minMotor = 0.05;
int motorArrays[2][3] = {{enA, in1, in2}, {enB, in4, in3}};
float motorMultipliers[2] = {0.875f, 1.0f};
float angles[4] = {0, 90, 180, 270};
int rotateAfterMillis = 5000;
int i = 0;

float angle = 0.0f;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
}

// between -1 and 1
void move(float x, char motorNum){
  analogWrite(motorArrays[motorNum][0], (int)(255 * motorMultipliers[motorNum] * abs(x)));
  
  if (x > minMotor){
    digitalWrite(motorArrays[motorNum][1], HIGH);
    digitalWrite(motorArrays[motorNum][2], LOW);
  }
  else if (x < -minMotor){    
    digitalWrite(motorArrays[motorNum][1], LOW);
    digitalWrite(motorArrays[motorNum][2], HIGH);
  }
  else{
    digitalWrite(motorArrays[motorNum][1], LOW);
    digitalWrite(motorArrays[motorNum][2], LOW);
  }
}

void rotate(float toAngle){
  float diff = toAngle - angle;
  Serial.print(angle); Serial.print(" - "); Serial.println(diff);
  if (diff > 2){
    move(+0.2f, 0);
    move(-0.2f, 1);
  }
  else if (diff < -2){
    move(-0.2f, 0);
    move(+0.2f, 1);
  }
  else{
    move(0.0f, 0);
    move(0.0f, 1);
  }
}

void loop() {
  mpu.update();
  angle = mpu.getAngleZ();

  if ((millis() - myTimer) > rotateAfterMillis) {
    i = (millis() / 1000) % 4;
    myTimer = millis();
  }

  rotate(angles[1]);
}
