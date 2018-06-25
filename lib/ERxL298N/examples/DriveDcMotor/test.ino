#include <L298N.h>

L298N motor1(2, 3);
L298N motor2(4, 5);
L298N motor3(6, 7);
L298N motor4(8, 9);

void setup()
{
	Serial.begin(9600);
	Serial.println("initializing...");
}

void stop()
{
	motor1.stop();
	motor2.stop();
	motor3.stop();
	motor4.stop();
}

void testrun(L298N &motor)
{
	stop();

	Serial.println("start forward...");
	motor.forward();
	delay(1000);
	Serial.println("end forward...");

	stop();

	delay(1000);

	Serial.println("start backward...");
	motor.backward();
	delay(1000);
	stop();
	Serial.println("end backward...");

	delay(1000);
}


void loop()
{
	testrun(motor1);
	testrun(motor2);
	testrun(motor3);
	testrun(motor4);
}
