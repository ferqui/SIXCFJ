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

int lastMovement = 7;
std::stack<int> Movements;
std::stack<int> PrintMov;

int CNY[] = {A0,A1,A5}; // CNYLeft, CNYRight, CNYBottom
int Sharp[] = {A8,A4}; // SHarpLeft, SharpRight
int Ultrasonic[] = {A3,11};
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
  switch (State) {
    case Robot::waiting:

    break;

    case Robot::lookingforEXIT:
      lookforEXIT();
    break;

    case Robot::lookingforSTART:
      lookforSTART();
    break;
  }/*
  delay(3000);
  testSensors();*/
}

void lookforEXIT() {
  while (!isTheEnd()) {
    int allowedMovement = checkWalls();
    testSensors();
    if (allowedMovement & RIGHT & lastMovement) {
      Serial1.println("-----------------");
      Serial1.println("RIGHT");
      Serial1.println("-----------------");
      //testSensors();
      Movements.push(RIGHT);
      robot.Move(Robot::right,200);
      robot.Move(Robot::forward,200);
      robot.Move(Robot::stoprobot,0);
      lastMovement = 7; delay(3000);
    } else {
      if (allowedMovement & FORWARD & lastMovement) {
        Serial1.println("-----------------");
        Serial1.println("FORWARD");
        Serial1.println("-----------------");
        //testSensors();
        Movements.push(FORWARD);
        robot.Move(Robot::forward,200);
        robot.Move(Robot::stoprobot,0);
        lastMovement = 7; delay(3000);
      } else {
        if (allowedMovement & LEFT & lastMovement) {
          Serial1.println("-----------------");
          Serial1.println("LEFT");
          Serial1.println("-----------------");
          //testSensors();
          Movements.push(LEFT);
          robot.Move(Robot::left,200);
          robot.Move(Robot::forward,200);
          robot.Move(Robot::stoprobot,0);
          lastMovement = 7; delay(3000);
        } else {
          if (!Movements.empty()) {
            Serial1.println(Movements.top());
            if (Movements.top() & FORWARD) {
              Serial1.println("-----------------");
              Serial1.println("BACK FORWARD");
              Serial1.println("-----------------");
              //testSensors();
              Movements.pop();
              robot.Move(Robot::backward,200);
              robot.Move(Robot::stoprobot,0);
              lastMovement = 4; delay(3000);
            } else {
              if (Movements.top() & RIGHT) {
                Serial1.println("-----------------");
                Serial1.println("BACK RIGHT");
                Serial1.println("-----------------");
                //testSensors();
                Movements.pop();
                robot.Move(Robot::backward,200);
                robot.Move(Robot::rightbackward,200);
                robot.Move(Robot::stoprobot,0);
                lastMovement = 6; delay(3000);
              } else { // LEFT
                Serial1.println("-----------------");
                Serial1.println("BACK LEFT");
                Serial1.println("-----------------");
                //testSensors();
                Movements.pop();
                robot.Move(Robot::backward,200);
                robot.Move(Robot::leftbackward,200);
                robot.Move(Robot::stoprobot,0);
                lastMovement = 0; delay(3000);
              }
            }
          }
        }
      }
    }
  }
  robot.Move(Robot::stoprobot,0);
    robot.TurnOnLed('R',254); delay(2000); robot.TurnOnLed('G',254);  delay(2000);
    robot.TurnOnLed('G',0); delay(2000); robot.TurnOnLed('G',254); delay(1000); robot.TurnOnLed('D',0);
    delay(50);
  
  robot.TurnOnLed('R',0);
  robot.TurnOnLed('G',0);
  State = Robot::lookingforSTART;
  
}

void lookforSTART() {
  int mov;
  while(!Movements.empty()){
     mov = robot.ReverseMov(Movements.top());
     switch(mov){
        case RIGHT_BACKWARD:
           robot.Move(Robot::backward,200);
           robot.Move(Robot::rightbackward,200);
           robot.Move(Robot::stoprobot,0);
           delay(3000);
          break;
        case LEFT_BACKWARD:
           robot.Move(Robot::backward,200);
           robot.Move(Robot::leftbackward,200);
           robot.Move(Robot::stoprobot,0);
           delay(3000);
          break;
        case BACKWARD:
           robot.Move(Robot::backward,200);
           robot.Move(Robot::stoprobot,0);
           delay(3000);
          break;

        default:
          break;
     }
     Movements.pop();
  }
}

int checkWalls() {
  int result = 0;
  if (13 < robot.ReadSharp('L') || robot.ReadSharp('L') < 0) {
    result |= LEFT;
  }
  if (13 < robot.ReadSharp('R') || robot.ReadSharp('R') < 0) {
    result |= RIGHT;
  }
  if (10 < robot.ReadUltrasonic()) {
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
  Serial1.print("Battery: "); Serial1.println(String(robot.BatteryState(),2));
  /*
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
  Serial1.print("Sharp R: "); Serial1.println(robot.ReadSharp('R'));*/
  Serial1.println("~~~~~~~~~~~~~~~~~~~~");
  Serial1.print("Stack: "); 
  while(!Movements.empty()){
    Serial1.print(Movements.top()); Serial1.print(", ");
    PrintMov.push(Movements.top());
    Movements.pop();
  }
  
  while(!PrintMov.empty()){
    Movements.push(PrintMov.top());
    PrintMov.pop();
  }
}
