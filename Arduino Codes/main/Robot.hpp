#ifndef ROBOT_H
#define ROBOT_H

#include "Arduino.h"

class Robot {

public:
	enum tState {waiting, lookingforEXIT, lookingforSTART};
	enum tMove1 {forward, backward, right, left, rightbackward, leftbackward, turn_back, stoprobot};
	enum tMove2 {E_forward, E_backward, E_right, E_left, E_rightbackward, E_leftbackward};

	Robot(int* CNY, int* Sharp, int* Ultrasonic, int* MotorL, int* MotorR,
		int* Led, int Battery, tState State, int left_encoder, int right_encoder):
		CNY{CNY}, Sharp{Sharp}, Ultrasonic{Ultrasonic}, MotorL{MotorL},
		MotorR{MotorR}, Led{Led}, Battery{Battery}, State{State},
		left_encoder{left_encoder}, right_encoder{right_encoder} {};

  void init();

	void Move(Robot::tMove1, int speed);
	void MoveEncoder(Robot::tMove2, int speedL, int speedR, float nT);
	void MoveAbsolute(char direction, int speedL, int speedR);
	void TurnOnLed(char color, int intensity);
	bool ReadCNY(char CNY);
	float ReadSharp(char sharp);
	float ReadUltrasonic();
	void WriteBT();
  String ReadBT();
  void set_State(tState s) {State=s;}
	tState get_State() {return State;}
  float BatteryState();
  void Encoder(bool state);

	~Robot();

private :
	int* CNY; // CNYLeft, CNYRight, CNYBottom
	int* Sharp; // SHarpLeft, SharpRight
	int* Ultrasonic; // Trigger, Echo
	int* MotorR;
	int* MotorL;
  int* Led; // Led1, Led2, Led3
  int Battery;
  int left_encoder;
  int right_encoder;
	tState State;

};

#endif
