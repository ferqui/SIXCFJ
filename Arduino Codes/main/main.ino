#include "Robot.hpp"
#include "Timer.h"

#define RIGHT 1
#define FORWARD 2
#define LEFT 4

Timer t;

int CNY[] = {A0,A1,A5}; // CNYLeft, CNYRight, CNYBottom
int Sharp[] = {A8,A4}; // SHarpLeft, SharpRight
int Ultrasonic = A3;
int MotorR[] = {5,6};
int MotorL[] = {9,10};
int Led[] = {12,13,11}; // Led1, Led2, Led3
int Battery = A6;

int pos[] = {0,0};
enum direction {f,b,l,r};
direction dir = f;
bool walls[25][25]; // Adjacency matrix
bool visited[5][5];

Robot::tState State = Robot::lookingforEXIT; // Must start in waiting

Robot robot(CNY,Sharp,Ultrasonic,MotorL,MotorR,Led,Battery,State,3,2);

void lookforEXIT();
void lookforSTART();
int checkWalls();
void move(int movement);
void continueStraight();
void radioControl();
bool isTheEnd();
void testSensors();

void handle_battery(){
  Serial1.print("bt" + String(robot.BatteryState(),2) + "#");
}

void setup() {
  Serial1.begin(9600); // Bluetooth
  robot.init();
  int tickEvent = t.every(500, handle_battery);
}

void loop() {
  t.update();
  switch (State) {
    case Robot::waiting:
      // Waiting to set the first position
    break;

    case Robot::lookingforEXIT:
      lookforEXIT();
    break;

    case Robot::lookingforSTART:
      lookforSTART();
    break;
  }
}

void lookforEXIT() {
  robot.Encoder(true);
  bool end = false;
  while (!end) {
    int movement = checkWalls();
    move(movement);
    end = isTheEnd();
  }
}

void lookforSTART() {
  /* These aren't the codes you are looking for... */
}

void move(int movement) {
  char priorityDirection[] = {'r','f','l','b'};

  bool choosen = false;
  int i = 0;
  while (i < sizeof(priorityDirection) && !choosen) {
    switch (priorityDirection[i]) {
      case 'f':
        if (movement & FORWARD) {
          choosen = true;
          continueStraight();
        }
      break;

      case 'b':
        robot.Move('b',150,150);
        continueStraight();
      break;

      case 'l':
        if  (movement & LEFT) {
          choosen = true;
          robot.Move('l',150,150);
          continueStraight();
        }
      break;

      case 'r':
        if (movement & RIGHT) {
          choosen = true;
          robot.Move('r',150,150);
          continueStraight();
        }
      break;
    }
    i++;
  }
}

int checkWalls() {
  int result = 0;
  switch (dir) {
    case f:
      if (robot.ReadSharp('L')>10) {
        walls[pos[0]-1 *5 + pos[1]][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0]-1 *5 + pos[1]] = 1;
        result |= LEFT;
      }
      if (robot.ReadSharp('R')>10) {
        walls[pos[0]+1 *5 + pos[1]][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0]+1 *5 + pos[1]] = 1;
        result |= RIGHT;
      }
      if (robot.ReadUltrasonic()>10) {
        walls[pos[0] *5 + pos[1]-1][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0] *5 + pos[1]-1] = 1;
        result |= FORWARD;
      }

    break;

    case b:
      if (robot.ReadSharp('L')>10) {
        walls[pos[0]+1 *5 + pos[1]][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0]+1 *5 + pos[1]] = 1;
        result |= LEFT;
      }
      if (robot.ReadSharp('R')>10) {
        walls[pos[0]-1 *5 + pos[1]][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0]-1 *5 + pos[1]] = 1;
        result |= RIGHT;
      }
      if (robot.ReadUltrasonic()>10) {
        walls[pos[0] *5 + pos[1]+1][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0] *5 + pos[1]+1] = 1;
        result |= FORWARD;
      }
    break;

    case l:
      if (robot.ReadSharp('L')>10) {
        walls[pos[0] *5 + pos[1]+1][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0] *5 + pos[1]+1] = 1;
        result |= LEFT;
      }
      if (robot.ReadSharp('R')>10) {
        walls[pos[0] *5 + pos[1]-1][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0] *5 + pos[1]-1] = 1;
        result |= RIGHT;
      }
      if (robot.ReadUltrasonic()>10) {
        walls[pos[0]-1 *5 + pos[1]][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0]-1 *5 + pos[1]] = 1;
        result |= FORWARD;
      }
    break;

    case r:
      if (robot.ReadSharp('L')>10) {
        walls[pos[0] *5 + pos[1]-1][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0] *5 + pos[1]-1] = 1;
        result |= LEFT;
      }
      if (robot.ReadSharp('R')>10) {
        walls[pos[0] *5 + pos[1]+1][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0] *5 + pos[1]+1] = 1;
        result |= RIGHT;
      }
      if (robot.ReadUltrasonic()>10) {
        walls[pos[0]+1 *5 + pos[1]][pos[0] *5 + pos[1]] = 1;
        walls[pos[0] *5 + pos[1]][pos[0]+1 *5 + pos[1]] = 1;
        result |= FORWARD;
      }
    break;
  }
  return result;
}

void continueStraight() { // With correction
  while (!robot.ReadCNY('B')) {
    if (robot.ReadSharp('L')-robot.ReadSharp('R')<-2) {
      robot.Move('f',170,150);
    } else {
      if (robot.ReadSharp('L')-robot.ReadSharp('R')>2) {
        robot.Move('f',150,170);
      } else {
        robot.Move('f',150,150);
      }
    }
  }
  delay(2000); // Go to the center of the cell
}

void radioControl() {
   robot.Encoder(false);
   if (Serial1.available() > 0 ) {
     char m = robot.ReadBT();
     robot.MoveAbsolute(m,150,150);
   }
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
