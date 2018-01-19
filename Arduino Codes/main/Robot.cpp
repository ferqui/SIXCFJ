#include "Robot.hpp"

void Robot::Move(char position, int speed){
  const float difference = 0.95;
  switch(position){
    case 'f': // Froward
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speed);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speed*difference);
      break;

    case 'b': // Back
      analogWrite(MotorR[0],speed);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],speed*difference);
      analogWrite(MotorL[1],LOW);
      break;

    case 'l': // Left
      analogWrite(MotorR[0],speed);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speed*difference);
      break;

    case 'r': // Right
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speed);
      analogWrite(MotorL[0],speed*difference);
      analogWrite(MotorL[1],LOW);
      break;

    default: // Stop
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
  }

}

void Robot::TurnOnLed(char color, int intensity){
  switch (color) {
    case 'R' : analogWrite(Led[0],intensity); break;
    case 'G' : analogWrite(Led[1],intensity); break;
    case 'B' : analogWrite(Led[2],intensity); break;
    default:
      analogWrite(Led[0],0);
      analogWrite(Led[1],0);
      analogWrite(Led[2],0);
  }
}

bool Robot::ReadCNY(char cny){
  int limit = 700;
  int value;
  switch (cny){
    case 'L': value = analogRead(CNY[0]); break;
    case 'R': value = analogRead(CNY[1]); break;
    case 'B': value = analogRead(CNY[2]); break;
    default: value = 0;
  }
  //Serial1.print(value);
  if (value>limit)
    return true; // Black
  else
    return false; // White
}

float Robot::ReadSharp(char sharp){
  const float ResolutionADC = 0.00488, /*4.88mV*/
              b = 0.024, m = 11.89;
  int Value_Sharp = 0;
  switch (sharp){
    case 'L': Value_Sharp = analogRead(Sharp[0]); break;
    case 'R': Value_Sharp = analogRead(Sharp[1]); break;
    default: Value_Sharp = 0;
  }
  float Voltage = Value_Sharp*ResolutionADC;
  float x = (Voltage-b)/m;
  return (1-0.42*x)/x;
}

float Robot::ReadUltrasonic(){
  float distance;
  unsigned long time_bounce;

  pinMode(Ultrasonic, OUTPUT);
  digitalWrite(Ultrasonic, LOW);
  delayMicroseconds(5);
  digitalWrite(Ultrasonic, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic, LOW);
  pinMode(Ultrasonic, INPUT);
  time_bounce = pulseIn(Ultrasonic, HIGH);
  distance = 0.017 * time_bounce;

  return distance;
}

char Robot::ReadBT(){
  String s = Serial1.readStringUntil('#');
  Serial1.read();
  return s[0];
}

float Robot::BatteryState(){
  return analogRead(Battery)*(5.00/1023.00)*2+0.7;
}

Robot::~Robot(){}
