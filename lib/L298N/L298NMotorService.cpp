#include "L298NMotorService.h"
#include <L298N.h>

// #define ENABLE_DEBUG_MESSAGE

#ifdef ENABLE_DEBUG_MESSAGE
#include <HardwareSerial.h>

template<class T>
void DEBUG_PRINT_LN(T message) 
{
	Serial.println(message); 
}

template<class T>
void DEBUG_PRINT(T message) 
{
	Serial.print(message); 
}

#else
// define the empty macros
#define DEBUG_PRINT_LN(message)
#define DEBUG_PRINT(message)
#endif

L298NMotorService::L298NMotorService()
{
	// initialize
	for(int i=0; i<MAX_MOTORS; i++){
		m_leftMotors[i]=0;
		m_rightMotors[i]=0;
	}
}

bool L298NMotorService::addLeftMotor(unsigned char e, unsigned char m, bool reverse/*=false*/){
	for(int i=0; i<MAX_MOTORS; i++){
		if(!m_leftMotors[i]){
			m_leftMotors[i] = new L298N(e, m, reverse);
			DEBUG_PRINT("add left motor: ");
			DEBUG_PRINT_LN(i);
			return true;
		}
	}
	return false;
}

bool L298NMotorService::addRightMotor(unsigned char e, unsigned char m, bool reverse/*=false*/){
	for(int i=0; i<MAX_MOTORS; i++){
		if(!m_rightMotors[i]){
			m_rightMotors[i] = new L298N(e, m, reverse);
			return true;
		}
	}
	return false;
}
void L298NMotorService::forward()
{
	DEBUG_PRINT("Move forward: ");

	for(int i=0; i<MAX_MOTORS; i++){
		if(m_leftMotors[i]){
			DEBUG_PRINT(" LF");DEBUG_PRINT(i);
			m_leftMotors[i]->forward();
		}
		if(m_rightMotors[i]){
			DEBUG_PRINT(" RF");DEBUG_PRINT(i);
			m_rightMotors[i]->forward();
		}
	}

	DEBUG_PRINT_LN(' ');
}
void L298NMotorService::backward()
{
	for(int i=0; i<MAX_MOTORS; i++){
		if(m_leftMotors[i]){
			m_leftMotors[i]->backward();
		}
		if(m_rightMotors[i]){
			m_rightMotors[i]->backward();
		}
	}
}
void L298NMotorService::turnLeft()
{
	DEBUG_PRINT("Turn left: ");
	
	for(int i=0; i<MAX_MOTORS; i++){
		if(m_leftMotors[i]){
			DEBUG_PRINT(" LB");DEBUG_PRINT(i);
			m_leftMotors[i]->backward();
		}
		if(m_rightMotors[i]){
			DEBUG_PRINT(" RF");DEBUG_PRINT(i);
			m_rightMotors[i]->forward();
		}
	}

	DEBUG_PRINT_LN(' ');
}
void L298NMotorService::turnRight()
{
	for(int i=0; i<MAX_MOTORS; i++){
		if(m_leftMotors[i]){
			m_leftMotors[i]->forward();
		}
		if(m_rightMotors[i]){
			m_rightMotors[i]->backward();
		}
	}
}
void L298NMotorService::stop()
{
	for(int i=0; i<MAX_MOTORS; i++){
		if(m_leftMotors[i]){
			m_leftMotors[i]->stop();
		}
		if(m_rightMotors[i]){
			m_rightMotors[i]->stop();
		}
	}
}

bool L298NMotorService::execute(unsigned int commandId)
{
	bool handled = true;
	/*
	// Command Format 
	<CMD_ROBOT_FORWARD><CR><LF>
	<CMD_ROBOT_BACKWARD><CR><LF>
	<CMD_ROBOT_TURN_RIGHT><CR><LF>
	<CMD_ROBOT_TURN_LEFT><CR><LF>
	<CMD_ROBOT_STOP><CR><LF>
	*/

	switch (commandId)
	{
	case CMD_ROBOT_FORWARD: 
		{
			forward();
		}
		break;
	case CMD_ROBOT_BACKWARD: 
		{
			backward();
		}
		break;
	case CMD_ROBOT_TURN_RIGHT: 
		{
			turnRight();
		}
		break;
	case CMD_ROBOT_TURN_LEFT: 
		{
			turnLeft();
		}
		break;
	case CMD_ROBOT_STOP: 
		{
			stop();
		}
		break;
	default:
		handled = false;
		break;
	}
	return handled;
}