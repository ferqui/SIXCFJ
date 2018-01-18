#ifndef ROBOT_H
#define ROBOT_H

#include "Arduino.h"

class Robot {

public:
	Robot(int* CNY, int* Sharp, int Ultrasonic, int* MotorL, int* MotorR,
		int* Led, int Battery, bool lookingforEXIT): CNY{CNY}, Sharp{Sharp}, Ultrasonic{Ultrasonic}, MotorL{MotorL},
		MotorR{MotorR}, Led{Led}, Battery{Battery}, lookingforEXIT{lookingforEXIT} {};

	void Move(char direction, int speed);
	void TurnOnLed(char color, int intensity);
	bool ReadCNY(char CNY);
	float ReadSharp(char sharp);
	float ReadUltrasonic();
	void WriteBT();
  char ReadBT();
  void islookingforEXIT(bool state){ lookingforEXIT = state;};
  bool islookingforEXIT(){ return lookingforEXIT;};
  float BatteryState();

	~Robot();

private :
	int* CNY; // CNYLeft, CNYRight, CNYBottom
	int* Sharp; // SHarpLeft, SharpRight
	int Ultrasonic;
	int* MotorR;
	int* MotorL;
  int* Led; // Led1, Led2, Led3
  int Battery;
  bool lookingforEXIT;

};

#endif
