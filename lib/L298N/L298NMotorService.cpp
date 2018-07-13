#include "L298NMotorService.h"
#include <L298N.h>

// #define ENABLE_DEBUG_MESSAGE

#ifdef ENABLE_DEBUG_MESSAGE
#include <HardwareSerial.h>

template <class T>
void DEBUG_PRINT_LN(T message)
{
	Serial.println(message);
}

template <class T>
void DEBUG_PRINT(T message)
{
	Serial.print(message);
}

#else
// define the empty macros
#define DEBUG_PRINT_LN(message)
#define DEBUG_PRINT(message)
#endif

L298NMotorService::L298NMotorService(bool pwsSpeed)
	: state(0), m_pwsSpeed(pwsSpeed)
{
	// initialize
	for (int i = 0; i < MAX_MOTORS; i++)
	{
		m_leftMotors[i] = 0;
		m_rightMotors[i] = 0;
	}

	for (int i = 0; i < MAX_SAFE_SENSOR; i++)
	{
		forwardSensorFuncs[i] = 0;
	}
}

bool L298NMotorService::addLeftMotor(unsigned char e, unsigned char m, bool reverse /*=false*/)
{
	for (int i = 0; i < MAX_MOTORS; i++)
	{
		if (!m_leftMotors[i])
		{
			m_leftMotors[i] = new L298N(e, m, reverse);
			DEBUG_PRINT("add left motor: ");
			DEBUG_PRINT_LN(i);
			return true;
		}
	}
	return false;
}

bool L298NMotorService::addRightMotor(unsigned char e, unsigned char m, bool reverse /*=false*/)
{
	for (int i = 0; i < MAX_MOTORS; i++)
	{
		if (!m_rightMotors[i])
		{
			m_rightMotors[i] = new L298N(e, m, reverse);
			return true;
		}
	}
	return false;
}

bool L298NMotorService::addForwardSafeSensor(Func func)
{
	for (int i = 0; i < MAX_SAFE_SENSOR; i++)
	{
		if (!forwardSensorFuncs[i])
		{
			forwardSensorFuncs[i] = func;
			DEBUG_PRINT("add forward safe sensor: ");
			DEBUG_PRINT_LN(i);
			return true;
		}
	}
	return false;
}

void L298NMotorService::forward(unsigned int speed)
{
	DEBUG_PRINT("Move forward: ");
	for (int i = 0; i < MAX_SAFE_SENSOR; i++)
	{
		if (forwardSensorFuncs[i] != 0)
		{
			if (forwardSensorFuncs[i]())
			{
				DEBUG_PRINT_LN("sensor check stop");
				stop(speed);
				return;
			}
		}
	}

	for (int i = 0; i < MAX_MOTORS; i++)
	{
		if (m_leftMotors[i])
		{
			DEBUG_PRINT(" LF");
			DEBUG_PRINT(i);
			m_leftMotors[i]->forward(speed);
		}
		if (m_rightMotors[i])
		{
			DEBUG_PRINT(" RF");
			DEBUG_PRINT(i);
			m_rightMotors[i]->forward(speed);
		}
	}
	state = STATE_FORWARD;
	DEBUG_PRINT_LN(' ');
}
void L298NMotorService::backward(unsigned int speed)
{
	DEBUG_PRINT("Move backward: ");

	for (int i = 0; i < MAX_MOTORS; i++)
	{
		if (m_leftMotors[i])
		{
			m_leftMotors[i]->backward(speed);
		}
		if (m_rightMotors[i])
		{
			m_rightMotors[i]->backward(speed);
		}
	}
	state = STATE_BACKWORD;
	DEBUG_PRINT_LN(' ');
}
void L298NMotorService::turnLeft(unsigned int speed)
{
	DEBUG_PRINT("Turn left: ");

	for (int i = 0; i < MAX_MOTORS; i++)
	{
		if (m_leftMotors[i])
		{
			DEBUG_PRINT(" LB");
			DEBUG_PRINT(i);
			m_leftMotors[i]->backward(speed);
		}
		if (m_rightMotors[i])
		{
			DEBUG_PRINT(" RF");
			DEBUG_PRINT(i);
			m_rightMotors[i]->forward(speed);
		}
	}
	state = STATE_TURN_LEFT;
	DEBUG_PRINT_LN(' ');
}
void L298NMotorService::turnRight(unsigned int speed)
{
	DEBUG_PRINT("Turn right: ");

	for (int i = 0; i < MAX_MOTORS; i++)
	{
		if (m_leftMotors[i])
		{
			DEBUG_PRINT(" LF");
			DEBUG_PRINT(i);
			m_leftMotors[i]->forward(speed);
		}
		if (m_rightMotors[i])
		{
			DEBUG_PRINT(" RB");
			DEBUG_PRINT(i);
			m_rightMotors[i]->backward(speed);
		}
	}
	state = STATE_TURN_RIGHT;
	DEBUG_PRINT_LN(' ');
}
void L298NMotorService::stop(unsigned int speed)
{
	DEBUG_PRINT("Stop: ");

	for (int i = 0; i < MAX_MOTORS; i++)
	{
		if (m_leftMotors[i])
		{
			m_leftMotors[i]->stop(speed);
		}
		if (m_rightMotors[i])
		{
			m_rightMotors[i]->stop(speed);
		}
	}
	state = STATE_STOP;
	DEBUG_PRINT_LN(' ');
}

bool L298NMotorService::execute(unsigned int commandId, unsigned int speed)
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
		forward(speed);
	}
	break;
	case CMD_ROBOT_BACKWARD:
	{
		backward(speed);
	}
	break;
	case CMD_ROBOT_TURN_RIGHT:
	{
		turnRight(speed);
	}
	break;
	case CMD_ROBOT_TURN_LEFT:
	{
		turnLeft(speed);
	}
	break;
	case CMD_ROBOT_STOP:
	{
		stop(speed);
	}
	break;
	default:
		handled = false;
		break;
	}
	return handled;
}