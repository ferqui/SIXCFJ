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

void Robot::Move(Robot::tMove1 mov, int speed) {
  const float difference = 0.75;
  rightCount=0;
  leftCount=0;
  switch (mov) {
    case Robot::forward: // Forward
      EncoderState = false;
      while (!ReadCNY('B')) {
        int basura = ReadUltrasonic();
        if (ReadSharp('L') < 12 && ReadSharp('R') < 12) { // Two Walls
          if (ReadSharp('L')-ReadSharp('R') < -2) {
            MoveAbsolute('f',speed,speed*difference);
          } else {
            if (ReadSharp('L')-ReadSharp('R') > 2) {
              MoveAbsolute('f',speed*difference,speed);
            } else {
               MoveAbsolute('f',speed,speed);
            }
          }
        } else {
          if (ReadSharp('L') < 12 && ReadSharp('R') > 12 && ReadSharp('R') < 20) { // Left Wall
            if (ReadSharp('L') < 4) {
              MoveAbsolute('f',speed,speed*difference);
            } else {
              if (ReadSharp('L') > 8) {
                MoveAbsolute('f',speed*difference,speed);
              } else {
                MoveAbsolute('f',speed,speed);
              }
            }
          } else {
            if (ReadSharp('L') > 12 && ReadSharp('L') < 20 && ReadSharp('R') < 12) { // Right Wall
              if (ReadSharp('R') < 4) {
                MoveAbsolute('f',speed*difference,speed);
              } else {
                if (ReadSharp('R') > 8) {
                  MoveAbsolute('f',speed,speed*difference);
                } else {
                  MoveAbsolute('f',speed,speed);
                }
              }
            } else { // Now Walls
              MoveAbsolute('f',speed,speed);
            }
          }
        }
      }
      while (!ReadCNY('R') && !ReadCNY('L') && !(ReadUltrasonic()>2 && ReadUltrasonic()<3)) {
        Serial1.println(ReadUltrasonic());
        if (ReadSharp('L') < 12 && ReadSharp('R') < 12) { // Two Walls
          if (ReadSharp('L')-ReadSharp('R') < -2) {
            MoveAbsolute('f',speed,speed*difference);
          } else {
            if (ReadSharp('L')-ReadSharp('R') > 2) {
              MoveAbsolute('f',speed*difference,speed);
            } else {
               MoveAbsolute('f',speed,speed);
            }
          }
        } else {
          if (ReadSharp('L') < 12 && ReadSharp('R') > 12 && ReadSharp('R') < 20) { // Left Wall
            if (ReadSharp('L') < 4) {
              MoveAbsolute('f',speed,speed*difference);
            } else {
              if (ReadSharp('L') > 8) {
                MoveAbsolute('f',speed*difference,speed);
              } else {
                MoveAbsolute('f',speed,speed);
              }
            }
          } else {
            if (ReadSharp('L') > 12 && ReadSharp('L') < 20 && ReadSharp('R') < 12) { // Right Wall
              if (ReadSharp('R') < 4) {
                MoveAbsolute('f',speed*difference,speed);
              } else {
                if (ReadSharp('R') > 8) {
                  MoveAbsolute('f',speed,speed*difference);
                } else {
                  MoveAbsolute('f',speed,speed);
                }
              }
            } else { // Now Walls
              MoveAbsolute('f',speed,speed);
            }
          }
        }
      }
      EncoderState = true;
      MoveEncoder(Robot::E_backward,speed,speed,2);
    break;

    case Robot::backward: // Backward
      EncoderState = false;
      while (!ReadCNY('R') && !ReadCNY('L')) {
        if (ReadSharp('L') < 12 && ReadSharp('R') < 12) { // Two Walls
          if (ReadSharp('L')-ReadSharp('R') < -2) {
            MoveAbsolute('b',speed,speed*difference);
          } else {
            if (ReadSharp('L')-ReadSharp('R') > 2) {
              MoveAbsolute('b',speed*difference,speed);
            } else {
               MoveAbsolute('b',speed,speed);
            }
          }
        } else {
          if (ReadSharp('L') < 12 && ReadSharp('R') > 12 && ReadSharp('R') < 20) { // Left Wall
            if (ReadSharp('L') < 4) {
              MoveAbsolute('b',speed,speed*difference);
            } else {
              if (ReadSharp('L') > 6) {
                MoveAbsolute('b',speed*difference,speed);
              } else {
                MoveAbsolute('b',speed,speed);
              }
            }
          } else {
            if (ReadSharp('L') > 12 && ReadSharp('L') < 20 && ReadSharp('R') < 12) { // Right Wall
              if (ReadSharp('R') < 4) {
                MoveAbsolute('b',speed*difference,speed);
              } else {
                if (ReadSharp('R') > 6) {
                  MoveAbsolute('b',speed,speed*difference);
                } else {
                  MoveAbsolute('b',speed,speed);
                }
              }
            } else { // Now Walls
              MoveAbsolute('b',speed,speed);
            }
          }
        }
      }
      while (!ReadCNY('B')) {
        if (ReadSharp('L') < 12 && ReadSharp('R') < 12) { // Two Walls
          if (ReadSharp('L')-ReadSharp('R') < -2) {
            MoveAbsolute('b',speed,speed*difference);
          } else {
            if (ReadSharp('L')-ReadSharp('R') > 2) {
              MoveAbsolute('b',speed*difference,speed);
            } else {
               MoveAbsolute('b',speed,speed);
            }
          }
        } else {
          if (ReadSharp('L') < 12 && ReadSharp('R') > 12 && ReadSharp('R') < 20) { // Left Wall
            if (ReadSharp('L') < 4) {
              MoveAbsolute('b',speed,speed*difference);
            } else {
              if (ReadSharp('L') > 6) {
                MoveAbsolute('b',speed*difference,speed);
              } else {
                MoveAbsolute('b',speed,speed);
              }
            }
          } else {
            if (ReadSharp('L') > 12 && ReadSharp('L') < 20 && ReadSharp('R') < 12) { // Right Wall
              if (ReadSharp('R') < 4) {
                MoveAbsolute('b',speed*difference,speed);
              } else {
                if (ReadSharp('R') > 6) {
                  MoveAbsolute('b',speed,speed*difference);
                } else {
                  MoveAbsolute('b',speed,speed);
                }
              }
            } else { // Now Walls
              MoveAbsolute('b',speed,speed);
            }
          }
        }
      }
      EncoderState = true;
      rightCount=0; leftCount=0;
      MoveEncoder(Robot::E_forward,speed,speed,2);
    break;

    case Robot::left: // Left
      MoveEncoder(Robot::E_leftbackward,speed,speed,2);
      MoveEncoder(Robot::E_backward,speed,speed,2);
      MoveEncoder(Robot::E_rightbackward,speed,speed,2);
      MoveEncoder(Robot::E_left,speed,speed,12);
    break;

    case Robot::right: // Right
      MoveEncoder(Robot::E_rightbackward,speed,speed,2);
      MoveEncoder(Robot::E_backward,speed,speed,2);
      MoveEncoder(Robot::E_leftbackward,speed,speed,2);
      MoveEncoder(Robot::E_right,speed,speed,12);
    break;

    case Robot::leftbackward: // Left
      MoveEncoder(Robot::E_leftbackward,speed,speed,2);
      MoveEncoder(Robot::E_forward,speed,speed,3);
      MoveEncoder(Robot::E_rightbackward,speed,speed,2);
      MoveEncoder(Robot::E_leftbackward,speed,speed,12);
    break;

    case Robot::rightbackward: // Right
      MoveEncoder(Robot::E_rightbackward,speed,speed,2);
      MoveEncoder(Robot::E_forward,speed,speed,3);
      MoveEncoder(Robot::E_leftbackward,speed,speed,2);
      MoveEncoder(Robot::E_rightbackward,speed,speed,12);
    break;

    case Robot::turn_back:
      MoveEncoder(Robot::E_rightbackward,speed,speed,3);
      MoveEncoder(Robot::E_backward,speed,speed,4);
      MoveEncoder(Robot::E_leftbackward,speed,speed,3);
      MoveEncoder(Robot::E_forward,speed,speed,4);
      rightCount=0; leftCount=0; nTicks = 10;
      analogWrite(MotorR[0],speed);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speed);
      while((rightCount < nTicks) || (leftCount < nTicks)){}
    break;

    default: // Stop
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
  }
}

void Robot::MoveEncoder(Robot::tMove2 mov, int speedL, int speedR, float nT) {
  nTicks = nT;
  rightCount=0;
  leftCount=0;
  switch (mov) {
    case Robot::E_forward:
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL);
      while((rightCount < nTicks) || (leftCount < nTicks)){}
    break;

    case Robot::E_backward: // Back
      analogWrite(MotorR[0],speedR);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],speedL);
      analogWrite(MotorL[1],LOW);
      while((rightCount < nTicks) || (leftCount < nTicks)){}
    break;

    case Robot::E_right:
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL);
      while(leftCount < nTicks){}
    break;

    case Robot::E_left:
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
      while(rightCount < nTicks){}
    break;

    case Robot::E_rightbackward:
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],speedL);
      analogWrite(MotorL[1],LOW);
      while(leftCount < nTicks){}
    break;

    case Robot::E_leftbackward:
      analogWrite(MotorR[0],speedR);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
      while(rightCount < nTicks){}
    break;

    default: // Stop
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],LOW);
  }
}


void Robot::MoveAbsolute(char position, int speedL, int speedR) {
  switch (position) {
    case 'f': // Forward
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL);
      break;

    case 'b': // Back
      analogWrite(MotorR[0],speedR);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],speedL);
      analogWrite(MotorL[1],LOW);
      break;

    case 'l': // Left
      analogWrite(MotorR[0],LOW);
      analogWrite(MotorR[1],speedR);
      analogWrite(MotorL[0],speedL);
      analogWrite(MotorL[1],LOW);
      break;

    case 'r': // Right
      analogWrite(MotorR[0],speedR);
      analogWrite(MotorR[1],LOW);
      analogWrite(MotorL[0],LOW);
      analogWrite(MotorL[1],speedL);
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
