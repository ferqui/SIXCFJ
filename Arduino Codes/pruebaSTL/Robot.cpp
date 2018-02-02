#include "Robot.hpp"

volatile float nTicks = 0;

volatile unsigned long leftCount = 0;
volatile unsigned long rightCount = 0;

int* LEFT;
int* RIGHT;
bool EncoderState = true;

void Robot::Encoder(bool state){
  EncoderState = state;
}

void right_handle() {
  if (RIGHT != NULL && EncoderState) {
    if (rightCount >= nTicks) {
      analogWrite(RIGHT[0],LOW);
      analogWrite(RIGHT[1],LOW);
    }
    ++rightCount;
  }
}

void left_handle() {
  if (LEFT != NULL && EncoderState) {
    if (leftCount >= nTicks) {
      analogWrite(LEFT[0],LOW);
      analogWrite(LEFT[1],LOW);
    }
    ++leftCount;
  }
}

void Robot::init() {
  LEFT = MotorL;
  RIGHT = MotorR;
  pinMode(MotorL[0], OUTPUT);
  pinMode(MotorL[1], OUTPUT);
  pinMode(MotorR[0], OUTPUT);
  pinMode(MotorR[1], OUTPUT);
  pinMode(Led[0], OUTPUT);
  pinMode(Led[1], OUTPUT);
  pinMode(Led[2], OUTPUT);
  pinMode(left_encoder, INPUT);
  pinMode(right_encoder, INPUT);

  attachInterrupt(digitalPinToInterrupt(left_encoder), left_handle, RISING);
  attachInterrupt(digitalPinToInterrupt(right_encoder), right_handle, RISING);
}

void Robot::MoveForward(float speedR, float speedL) {
  nTicks = 16;
  const float difference = 0.95;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],LOW);
  analogWrite(MotorR[1],speedR*difference);
  analogWrite(MotorL[0],LOW);
  analogWrite(MotorL[1],speedL);

  while((rightCount <= nTicks) || (leftCount <= nTicks)){}
}

void Robot::MoveBackLeft(float speedR, float speedL) {
  nTicks = 16;
  const float difference = 0.85;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],speedR*difference);
  analogWrite(MotorR[1],LOW);
  analogWrite(MotorL[0],speedL);
  analogWrite(MotorL[1],LOW);

  while((rightCount <= nTicks) || (leftCount <= nTicks)){}

  Serial.println("LeftCount: " + String(leftCount) + " RightCount: " + String(rightCount));

  nTicks = 10;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],speedR);
  analogWrite(MotorR[1],LOW);
  analogWrite(MotorL[0],LOW);
  analogWrite(MotorL[1],LOW);

  while(rightCount <= nTicks){}

  nTicks = 6;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],LOW);
  analogWrite(MotorR[1],speedR*difference);
  analogWrite(MotorL[0],LOW);
  analogWrite(MotorL[1],speedL);

  while((rightCount <= nTicks) || (leftCount <= nTicks)){}
}

void Robot::MoveBackRight(float speedR, float speedL) {
  nTicks = 16;
  const float difference = 0.85;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],speedR*difference);
  analogWrite(MotorR[1],LOW);
  analogWrite(MotorL[0],speedL);
  analogWrite(MotorL[1],LOW);

  while((rightCount <= nTicks) || (leftCount <= nTicks)){}

  Serial.println("LeftCount: " + String(leftCount) + " RightCount: " + String(rightCount));

  nTicks = 10;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],LOW);
  analogWrite(MotorR[1],LOW);
  analogWrite(MotorL[0],speedL);
  analogWrite(MotorL[1],LOW);

  while(leftCount <= nTicks){}

  nTicks = 6;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],LOW);
  analogWrite(MotorR[1],speedR*difference);
  analogWrite(MotorL[0],LOW);
  analogWrite(MotorL[1],speedL);

  while((rightCount <= nTicks) || (leftCount <= nTicks)){}
}

void Robot::MoveBackward(float speedR, float speedL) {
  nTicks = 16;
  const float difference = 0.85;
  rightCount=0;
  leftCount=0;

  analogWrite(MotorR[0],speedR*difference);
  analogWrite(MotorR[1],LOW);
  analogWrite(MotorL[0],speedL);
  analogWrite(MotorL[1],LOW);

  while((rightCount <= nTicks) || (leftCount <= nTicks)){}
}

void Robot::Move(char position, int speedL, int speedR, int nt) {
  const float difference = 0.95;
  rightCount=0;
  leftCount=0;
  nTicks = nt;
  switch (position) {
    case 'f': // Forward
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL*difference);
      break;

    case 'b': // Back
      //nTicks = 12;
      analogWrite(MotorR[0],speedR);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],speedL*difference);
      analogWrite(MotorL[1],LOW);
      while((rightCount <= nTicks) || (leftCount <= nTicks)){}
      break;

    case 'l': // Left
      //nTicks = 12;
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
      while(rightCount <= nTicks){}
      break;

    case 'r': // Right
      //nTicks = 12;
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL);
      while(leftCount <= nTicks){}
      break;

    default: // Stop
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
  }

}

void Robot::MoveAbsolute(char position, int speedL, int speedR) {
  const float difference = 0.95;
  switch (position) {
    case 'f': // Forward
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL*difference);
      break;

    case 'b': // Back
      analogWrite(MotorR[0],speedR);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],speedL*difference);
      analogWrite(MotorL[1],LOW);
      break;

    case 'l': // Left
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],speedL*difference);
      analogWrite(MotorL[1],LOW);
      break;

    case 'r': // Right
      analogWrite(MotorR[0],speedR);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL*difference);
      break;

    default: // Stop
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
  }

}

void Robot::TurnOnLed(char color, int intensity) {
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

bool Robot::ReadCNY(char cny) {
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

float Robot::ReadSharp(char sharp) {
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

float Robot::ReadUltrasonic() {
  float distance;
  unsigned long time_bounce;

  pinMode(Ultrasonic,OUTPUT);
  digitalWrite(Ultrasonic,LOW);
  delayMicroseconds(5);
  digitalWrite(Ultrasonic,HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic,LOW);
  pinMode(Ultrasonic,INPUT);
  time_bounce = pulseIn(Ultrasonic,HIGH);
  distance = 0.017 * time_bounce;

  return distance;
}

String Robot::ReadBT() {
  String s = Serial1.readStringUntil('#');
  Serial1.read();
  return s;
}

float Robot::BatteryState() {
  return analogRead(Battery)*(5.00/1023.00)*2+0.7;
}

Robot::~Robot(){}
