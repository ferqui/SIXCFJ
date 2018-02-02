#include <StandardCplusplus.h>
#include <stack>
#include "Robot.hpp"

const int FORWARD = 1;
const int LEFT = 2;
const int RIGHT = 4;
const int BACKWARD = 8;
const int LEFT_BACKWARD = 16;
const int RIGHT_BACKWARD = 32;

int lastMovement = 0;
std::stack<int> Movements;

int CNY[] = {A0,A1,A5}; // CNYLeft, CNYRight, CNYBottom
int Sharp[] = {A8,A4}; // SHarpLeft, SharpRight
int Ultrasonic = A3;
int MotorR[] = {5,6};
int MotorL[] = {9,10};
int Led[] = {12,13,11}; // Led1, Led2, Led3
int Battery = A6;

Robot::tState State = Robot::lookingforEXIT;

Robot robot(CNY,Sharp,Ultrasonic,MotorL,MotorR,Led,Battery,State,3,2);

void setup() {
  Serial1.begin(9600); // Bluetooth
  Serial.begin(9600);
  robot.init();

}

void loop() {
  /*Serial.println(checkWalls(),BIN);
  Serial.println(RIGHT,BIN);
  Serial.println(checkWalls() & RIGHT,BIN);
  Serial.println("*******************");
  delay(500);
  return;*/
  while(!isTheEnd()){
    int allowedMovement = checkWalls();
    Serial.println(allowedMovement,BIN);
    Serial.println(RIGHT,BIN);
    Serial.println(allowedMovement & RIGHT,BIN);
    Serial.println("*******************");
    delay(5000);
    if(Movements.empty()){
      if((lastMovement == 0) && (allowedMovement & RIGHT)){
        Serial1.println("Move right");
        robot.Move('r',100,100,16);
        Movements.push(RIGHT);
        lastMovement = RIGHT;
      }else if((lastMovement == RIGHT_BACKWARD) && (allowedMovement & FORWARD)){
        Serial1.println("Move forward");
        continueStraight();
        Movements.push(FORWARD);
        lastMovement = FORWARD;
      }else if((lastMovement == BACKWARD) && (allowedMovement & LEFT)){
        Serial1.println("Move left");
        robot.Move('r',100,100,16);
        Movements.push(LEFT);
        lastMovement = LEFT;
      }else{
        // Salir
        break;
      }
    }else{
      if((lastMovement != RIGHT_BACKWARD) && (lastMovement != BACKWARD) && (lastMovement != LEFT_BACKWARD) && (allowedMovement & RIGHT)){
        Serial1.println("Move right");
        robot.Move('r',100,100,16);
        Movements.push(RIGHT);
        lastMovement = RIGHT;
      }else if((lastMovement != BACKWARD) && (lastMovement != LEFT_BACKWARD) && (allowedMovement & FORWARD)){
        Serial1.println("Move forward");
        robot.MoveForward(100,100);
        Movements.push(FORWARD);
        lastMovement = FORWARD;
      }else if((lastMovement != LEFT_BACKWARD) && (allowedMovement & LEFT)){
        Serial1.println("Move left");
        robot.Move('l',100,100,16);
        Movements.push(LEFT);
        lastMovement = LEFT;
      }else{
        int ActualMovement = Movements.top();
        Movements.pop();
        switch(ActualMovement){
          case RIGHT:
            Serial1.println("Move back right");
             lastMovement = RIGHT_BACKWARD;
             robot.MoveBackRight(100,100);
             break;
          case FORWARD:
            Serial1.println("Move back");
             lastMovement = BACKWARD;
             robot.MoveBackward(100,100);
             break;
          case LEFT:
            Serial1.println("Move back right");
             lastMovement = LEFT_BACKWARD;
             robot.MoveBackLeft(100,100);
             break;
        }
      }
    }
  }
}

int checkWalls() {
  int result = 0;
  if (robot.ReadSharp('L')>12) {
    result |= LEFT;
  }
  if (robot.ReadSharp('R')>12) {
    result |= RIGHT;
  }
  if (robot.ReadUltrasonic()>12) {
    result |= FORWARD;
  }
  return result;
}

bool isTheEnd() { // All CNY are Black
  return robot.ReadCNY('L')&&robot.ReadCNY('R')&&robot.ReadCNY('B');
}

void continueStraight() { // With correction
  while (!robot.ReadCNY('B')) {
    if ((robot.ReadSharp('L')-robot.ReadSharp('R'))<-2) {
      robot.Move('f',170,150,5);
    } else {
      if ((robot.ReadSharp('L')-robot.ReadSharp('R'))>2) {
        robot.Move('f',150,170,5);
      } else {
        robot.Move('f',150,150,5);
      }
    }
  }
  // TODO: Use wheels encoders
  delay(1000); // Go to the center of the cell
  robot.Move(' ',0,0,0);
}

void testSensors() {
  if (robot.ReadCNY('L'))
    Serial.println("CNY L Black");
  else Serial.println("CNY L White");
  if (robot.ReadCNY('R'))
    Serial.println("CNY R Black");
  else Serial.println("CNY R White");
  if (robot.ReadCNY('B'))
    Serial.println("CNY B Black");
  else Serial.println("CNY B White");
  Serial.print("Ultrasonic: "); Serial.println(robot.ReadUltrasonic());
  Serial.print("Sharp L: "); Serial.println(robot.ReadSharp('L'));
  Serial.print("Sharp R: "); Serial.println(robot.ReadSharp('R'));
}
