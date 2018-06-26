#include "L298N.h"

L298N::L298N(unsigned char e, unsigned char m, bool reverse /*= false*/)
	:m_e(e), m_m(m), m_reverse(reverse)
{
	pinMode(m_e, OUTPUT);
	pinMode(m_m, OUTPUT);
	stop();
}

void L298N::forward(unsigned int speed /*= 0*/)
{
	digitalWrite(m_e, m_reverse ? HIGH : LOW);
	digitalWrite(m_m, m_reverse ? LOW : HIGH);
}

void L298N::backward(unsigned int speed /*= 0*/)
{
	digitalWrite(m_e, m_reverse ? LOW : HIGH);
	digitalWrite(m_m, m_reverse ? HIGH : LOW);
}

void L298N::stop()
{
	digitalWrite(m_e, LOW);
	//TODO haiyang.song 为什么只修改一个的状态？
	digitalWrite(m_m, LOW);
}