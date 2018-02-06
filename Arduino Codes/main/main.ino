#include <StandardCplusplus.h>
#include <stack>
#include "Timer.h"
#include "Robot.hpp"

const int RIGHT = 1;
const int FORWARD = 2;
const int LEFT = 4;
const int BACKWARD = 8;
const int LEFT_BACKWARD = 16;
const int RIGHT_BACKWARD = 32;

Timer t;

int lastMovement = 0;
std::stack<int> Movements;

int CNY[] = {A0,A1,A5}; // CNYLeft, CNYRight, CNYBottom
int Sharp[] = {A8,A4}; // SHarpLeft, SharpRight
int Ultrasonic = A3;
int MotorR[] = {5,6};
int MotorL[] = {9,10};
int Led[] = {12,13,11}; // Led1, Led2, Led3
int Battery = A6;

Robot::tState State = Robot::lookingforEXIT; // Must start in waiting

Robot robot(CNY,Sharp,Ultrasonic,MotorL,MotorR,Led,Battery,State,3,2);

void handle_battery(){
  //Serial1.print("bt" + String(pos[0]) + String(pos[1])+"#");
  Serial1.print("bt" + String(robot.BatteryState(),2) + "#");
}

void setup() {
  Serial1.begin(9600); // Bluetooth
  Serial.begin(9600);
  robot.init();
  int tickEvent = t.every(500, handle_battery);
}

void loop() {
  delay(10000);
  switch (State) {
    case Robot::waiting:

    break;

    case Robot::lookingforEXIT:
      lookforEXIT();
    break;

    case Robot::lookingforSTART:

    break;
  }
}

void lookforEXIT() {
  while (!isTheEnd()) {
    int allowedMovement = checkWalls();
    if (allowedMovement & RIGHT) {
      robot.Move(Robot::right,200);
      robot.Move(Robot::forward,200);
      robot.Move(Robot::stoprobot,0); testSensors(); //delay(5000);
    } else {
      if (allowedMovement & FORWARD) {
        robot.Move(Robot::forward,200);
        robot.Move(Robot::stoprobot,0); testSensors(); //delay(5000);
      } else {
        if (allowedMovement & LEFT) {
          robot.Move(Robot::left,200);
          robot.Move(Robot::forward,200);
          robot.Move(Robot::stoprobot,0); testSensors(); //delay(5000);
        } else {
          robot.Move(Robot::turn_back,200);
          robot.Move(Robot::stoprobot,0); testSensors(); //delay(5000);
        }
      }
    }
  }
  robot.Move(Robot::stoprobot,0);
}

void lookforSTART() {

}

int checkWalls() {
  int result = 0;
  if (12 < robot.ReadSharp('L') || robot.ReadSharp('L') < 0) {
    result |= LEFT;
  }
  if (12 < robot.ReadSharp('R') || robot.ReadSharp('R') < 0) {
    result |= RIGHT;
  }
  if (5 < robot.ReadUltrasonic()) {
    result |= FORWARD;
  }
  return result;
}

void radioControl(String m) {
   robot.Encoder(false);
   //if (Serial1.available() > 0 ) {
     //String m = robot.ReadBT();
     robot.MoveAbsolute(m[0],150,150);
   //}
}

bool isTheEnd() { // All CNY are Black
  return robot.ReadCNY('L')&&robot.ReadCNY('R')&&robot.ReadCNY('B');
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
