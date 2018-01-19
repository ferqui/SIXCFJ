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

Robot robot(CNY,Sharp,Ultrasonic,MotorL,MotorR,Led,Battery,false,3,2);

void handle_battery(){
  Serial1.print("bt" + String(robot.BatteryState(),2) + "#");
}

void setup() {
  Serial1.begin(9600); // Bluetooth
  Serial.begin(9600);
  robot.init();
  int tickEvent = t.every(500, handle_battery);
}

void loop() {
  robot.Move('l',100);
  delay(5000);
  robot.Move('r',100);
  delay(5000);
  robot.Move('r',100);
  robot.Move('r',100);
  delay(2500);
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
