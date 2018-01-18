#ifndef ROBOT_H
#define ROBOT_H

#include "Arduino.h"

class Robot {

public:
	Robot(int CNYL, int CNYR, int CNYB, int SharpL, int SharpR, int UltraSound,
				int MotorL1, int MotorL2, int MotorR1, int MotorR2):
				CNYL{CNYL},CNYR{CNYR},CNYB{CNYB},SharpL{SharpL},	SharpR{SharpR},UltraSound{UltraSound},
				MotorL1{MotorL1},MotorL2{MotorL2},
				MotorR1{MotorR1}, MotorR2{MotorR2} {};
	void Move(char direction, int speed);
	void Led(char type);
	int ReadCNY(char CNY);
	float ReadSharp(char sharp);
	float ReadUltraSound();
	//float WriteBT();
  char ReadBT();

	~Robot();

private :
	int CNYL;
	int CNYR;
	int CNYB;
	int SharpL;
	int SharpR;
	int UltraSound;
	int MotorR1;
	int MotorR2;
	int MotorL1;
	int MotorL2;
};

#endif
