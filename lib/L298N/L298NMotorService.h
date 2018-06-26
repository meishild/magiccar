/*

***************************************
This service is deprecated by the MotorService.
***************************************

This service is for the motor driven robot car. 
There are two sets of motor on the both sides of car.

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

* Created 4 October 2014
* By Jeffrey Sun
* 

*/
#pragma once
#ifndef _L298N_MOTOR_SERVICE_H
#define _L298N_MOTOR_SERVICE_H


// Define the maximum motors for each set.
#define MAX_MOTORS 4

// 0x70 - 0x7F motor commands
#define CMD_ROBOT_FORWARD 0x70
#define CMD_ROBOT_BACKWARD 0x71
#define CMD_ROBOT_TURN_RIGHT 0x72
#define CMD_ROBOT_TURN_LEFT 0x73
#define CMD_ROBOT_STOP 0x74

class L298N;

class L298NMotorService
{
public:
	L298NMotorService();

public:

	// Execute the command saved in the context. The result is output to the context.
	bool execute(unsigned int commandId);

public:
	// Add a motor to the left set of the robot.
	bool addLeftMotor(unsigned char e, unsigned char m, bool reverse=false);
	// Add a motor to the right set of the robot.
	bool addRightMotor(unsigned char e, unsigned char m, bool reverse=false);

private:
	void forward();
	void backward();
	void turnLeft();
	void turnRight();
	void stop();

private:
	L298N *m_leftMotors[MAX_MOTORS];
	L298N *m_rightMotors[MAX_MOTORS];
};

#endif // _L298N_MOTOR_SERVICE_H