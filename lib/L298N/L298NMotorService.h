/*
Required hardware
Arduino
L298N motor driver

Execute
Executable command format
<CMD_ROBOT_FORWARD><CR><LF>
<CMD_ROBOT_BACKWARD><CR><LF>
<CMD_ROBOT_TURN_RIGHT><CR><LF>
<CMD_ROBOT_TURN_LEFT><CR><LF>
<CMD_ROBOT_STOP><CR><LF>

Result format
<SYS_RESULT> OK<CR><LF>

*/
#pragma once
#ifndef _L298N_MOTOR_SERVICE_H
#define _L298N_MOTOR_SERVICE_H

// Define the maximum motors for each set.
#define MAX_MOTORS 4

#define MAX_SAFE_SENSOR 4

// 0x70 - 0x7F motor commands
#define CMD_ROBOT_FORWARD 11
#define CMD_ROBOT_BACKWARD 12
#define CMD_ROBOT_TURN_RIGHT 13
#define CMD_ROBOT_TURN_LEFT 14
#define CMD_ROBOT_STOP 15

// #define ENABLE_DEBUG_MESSAGE

#define STATE_FORWARD 1
#define STATE_BACKWORD 2
#define STATE_TURN_RIGHT 3
#define STATE_TURN_LEFT 4
#define STATE_STOP 5

class L298N;

class L298NMotorService
{
  public:
	L298NMotorService();

  public:
	int state;

	typedef bool (*Func)(void);
	// Execute the command saved in the context. The result is output to the context.
	bool execute(unsigned int commandId,unsigned int speed=0);

  public:
	// Add a motor to the left set of the robot.
	bool addLeftMotor(unsigned char e, unsigned char m, bool reverse = false);
	// Add a motor to the right set of the robot.
	bool addRightMotor(unsigned char e, unsigned char m, bool reverse = false);
	// Add a motor direction faction.
	bool addForwardSafeSensor(Func func);

  private:
	void forward(unsigned int speed);
	void backward(unsigned int speed);
	void turnLeft(unsigned int speed);
	void turnRight(unsigned int speed);
	void stop(unsigned int speed);

  private:
	L298N *m_leftMotors[MAX_MOTORS];
	L298N *m_rightMotors[MAX_MOTORS];

	Func forwardSensorFuncs[MAX_SAFE_SENSOR];
};

#endif // _L298N_MOTOR_SERVICE_H