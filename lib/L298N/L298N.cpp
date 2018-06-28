#include "L298N.h"

L298N::L298N(unsigned char e, unsigned char m, bool reverse /*= false*/)
	: m_e(e), m_m(m), m_reverse(reverse)
{
	if (reverse)
	{
		m_e = m;
		m_m = e;
	}

	pinMode(m_e, OUTPUT);
	pinMode(m_m, OUTPUT);
	stop();
}

void L298N::forward(unsigned int speed /*= 0*/)
{
	if (speed == 0)
	{
		digitalWrite(m_e, LOW);
	}
	else
	{
		analogWrite(m_e, speed);
	}

	digitalWrite(m_m, HIGH);
}

void L298N::backward(unsigned int speed /*= 0*/)
{
	if (speed == 0)
	{
		digitalWrite(m_e, HIGH);
	}
	else
	{
		analogWrite(m_e, speed);
	}
	digitalWrite(m_m, LOW);
}

void L298N::stop(unsigned int speed /*= 0*/)
{
	if (speed == 0)
	{
		digitalWrite(m_e, LOW);
	}
	else
	{
		for (int i = speed; i >= 0; i--)
		{
			analogWrite(m_e, i);
		}
	}

	digitalWrite(m_m, LOW);
}