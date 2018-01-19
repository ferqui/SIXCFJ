#include "Robot.hpp"
#include "Timer.h"

Timer t;

int CNY[] = {A0,A1,A5}; // CNYLeft, CNYRight, CNYBottom
int Sharp[] = {A8,A4}; // SHarpLeft, SharpRight
int Ultrasonic = A3;
int MotorL[] = {5,6};
int MotorR[] = {9,10};
int Led[] = {12,13,11}; // Led1, Led2, Led3
int Battery = A6;

Robot robot(CNY,Sharp,Ultrasonic,MotorL,MotorR,Led,Battery,false);

void handle_battery(){
  float battery = map(robot.BatteryState(), 7.4, 9, 0, 100);
  battery = battery > 0 ? battery : 0;
  Serial1.print("bt" + String(battery,2) + "#");
  Serial.print("bt" + String(battery,2) + "#");
}

void setup() {
  Serial1.begin(9600); // Bluetooth
  Serial.begin(9600);
  pinMode(MotorL[0], OUTPUT);
  pinMode(MotorL[1], OUTPUT);
  pinMode(MotorR[0], OUTPUT);
  pinMode(MotorR[1], OUTPUT);s
  pinMode(Led[0], OUTPUT);
  pinMode(Led[1], OUTPUT);
  pinMode(Led[2], OUTPUT);

  int tickEvent = t.every(500, handle_battery);
}

void loop() {
  t.update();
  if (Serial1.available() > 0 ) {
    char m = robot.ReadBT();
    robot.Move(m,100);
  }
}
