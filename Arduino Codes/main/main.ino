#include "Robot.hpp"

const int CNYL = A0;
const int CNYR = A1;
const int CNYB = A5;
const int SharpL = A8;
const int SharpR = A4;
const int UltraSound = A3;
const int MotorL1 = 5;
const int MotorL2 = 6;

const int MotorR1 = 9;
const int MotorR2 = 10;

//led

Robot robot(CNYL,CNYR,CNYB,SharpL,SharpR,UltraSound,MotorL1,MotorL2,MotorR1,MotorR2);

void setup() {
  Serial1.begin(9600);
  pinMode(MotorL1, OUTPUT);
  pinMode(MotorL2, OUTPUT);
  pinMode(MotorR1, OUTPUT);
  pinMode(MotorR2, OUTPUT);
}

void loop() {
  char m = robot.ReadBT();
  robot.Move(m,100);
}
