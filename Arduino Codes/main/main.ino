#include "Robot.hpp"
#include "Timer.h"

Timer t;

int CNY[] = {A0,A1,A5}; // CNYLeft, CNYRight, CNYBottom
int Sharp[] = {A8,A4}; // SHarpLeft, SharpRight
int Ultrasonic = A3;
int MotorR[] = {5,6};
int MotorL[] = {9,10};
int Led[] = {12,13,11}; // Led1, Led2, Led3
int Battery = A6;

Robot robot(CNY,Sharp,Ultrasonic,MotorL,MotorR,Led,Battery,false);

void handle_battery(){
  Serial1.print("bt" + String(robot.BatteryState(),2) + "#");
}

void setup() {
  Serial1.begin(9600); // Bluetooth
  Serial.begin(9600);
  pinMode(MotorL[0], OUTPUT);
  pinMode(MotorL[1], OUTPUT);
  pinMode(MotorR[0], OUTPUT);
  pinMode(MotorR[1], OUTPUT);
  pinMode(Led[0], OUTPUT);
  pinMode(Led[1], OUTPUT);
  pinMode(Led[2], OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  int tickEvent = t.every(500, handle_battery);

  attachInterrupt(digitalPinToInterrupt(2), handle_rightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(3), handle_leftEncoder, RISING);
}

volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;

void handle_leftEncoder() {
  if(leftCount >= 12){
    analogWrite(MotorL[0],LOW);
    analogWrite(MotorL[1],LOW);
  }
  ++leftCount;
}

void handle_rightEncoder() {
  Serial.println(rightCount);
  if(rightCount >= 12){
    analogWrite(MotorR[0],LOW);
    analogWrite(MotorR[1],LOW);
  }
  ++rightCount;
}

bool isRunning = false;

void loop() {
  if(!isRunning) {
    isRunning = true;
    analogWrite(MotorR[0],LOW);
    analogWrite(MotorR[1],100);
    analogWrite(MotorL[0],LOW);
    analogWrite(MotorL[1],100);
  }
  t.update();
  radioControl();
}

void radioControl() {
   if (Serial1.available() > 0 ) {
     char m = robot.ReadBT();
     robot.Move(m,100);
   }
}

void testSensors() {
  if (robot.ReadCNY('L'))
    Serial1.println("CNY L Black");
  else Serial1.println("CNY L White");
  if (robot.ReadCNY('R'))
    Serial1.println("CNY R Black");
  else Serial1.println("CNY R White");
  if (robot.ReadCNY('B'))
    Serial1.println("CNY B Black");
  else Serial1.println("CNY B White");
  Serial1.print("Ultrasonic: "); Serial1.println(robot.ReadUltrasonic());
  Serial1.print("Sharp L: "); Serial1.println(robot.ReadSharp('L'));
  Serial1.print("Sharp R: "); Serial1.println(robot.ReadSharp('R'));
}
