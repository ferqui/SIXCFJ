#include "Robot.hpp"

int CNY[] = {A0,A1,A5}; // CNYLeft, CNYRight, CNYBottom
int Sharp[] = {A8,A4}; // SHarpLeft, SharpRight
int Ultrasonic = A3;
int MotorL[] = {5,6};
int MotorR[] = {9,10};
int Led[] = {12,13,11}; // Led1, Led2, Led3
int Battery = A6;

Robot robot(CNY,Sharp,Ultrasonic,MotorL,MotorR,Led,Battery,false);

void setup() {
  Serial1.begin(9600); // Bluetooth
  pinMode(MotorL[0], OUTPUT);
  pinMode(MotorL[1], OUTPUT);
  pinMode(MotorR[0], OUTPUT);
  pinMode(MotorR[1], OUTPUT);
  pinMode(Led[0], OUTPUT);
  pinMode(Led[1], OUTPUT);
  pinMode(Led[2], OUTPUT);
}

void loop() {
  if (Serial1.available() > 0 ) {
    char m = robot.ReadBT();
    robot.Move(m,100);
  }
}
