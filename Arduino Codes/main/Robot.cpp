#include "Robot.hpp"

void Robot::Move(char position, int speed){
  const float disminucion = 0.95;
  switch(position){
    case 'f':
      analogWrite(MotorR1,LOW);
      analogWrite(MotorR2,speed);
      analogWrite(MotorL1,LOW);
      analogWrite(MotorL2,speed*disminucion);
      break;

    case 'b':
      analogWrite(MotorR1,speed);
      analogWrite(MotorR2,LOW);
      analogWrite(MotorL1,speed*disminucion);
      analogWrite(MotorL2,LOW);
      break;

    case 'l':
      analogWrite(MotorR1,speed);
      analogWrite(MotorR2,LOW);
      analogWrite(MotorL1,LOW);
      analogWrite(MotorL2,speed*disminucion);
      break;

    case 'r':
      analogWrite(MotorR1,LOW);
      analogWrite(MotorR2,speed);
      analogWrite(MotorL1,speed*disminucion);
      analogWrite(MotorL2,LOW);
      break;

    case 's':
      analogWrite(MotorR1,LOW);
      analogWrite(MotorR2,LOW);
      analogWrite(MotorL1,LOW);
      analogWrite(MotorL2,LOW);
      break;
  }

}

void Robot::Led(char color){
  /*   */
}

int Robot::ReadCNY(char CNY){
  int value;
  switch (CNY){
    case 'L': value = analogRead(CNYL); break;
    case 'R': value = analogRead(CNYR); break;
    case 'B': value = analogRead(CNYB); break;
    default: value = 0;
  }
  return value;
}

float Robot::ReadSharp(char sharp){
  const float ResolutionADC = 0.00488, /*4.88mV*/
              b = 0.024, m = 11.89;
  int Value_Sharp = 0;
  switch (sharp){
    case 'L': Value_Sharp = analogRead(SharpL); break;
    case 'R': Value_Sharp = analogRead(SharpR); break;
    default: Value_Sharp = 0;
  }
  float Voltage = Value_Sharp*ResolutionADC;
  float x = (Voltage-b)/m;
  return (1-0.42*x)/x;
}

char Robot::ReadBT(){
  if (Serial1.available() > 0 ) {
    String s = Serial1.readStringUntil('#');
    Serial1.read();
    return s[0];
  }
  return 's';
}

Robot::~Robot(){}
