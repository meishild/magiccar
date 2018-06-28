/*
Function
* This library is used to control L298N DC motor driver.
* It only controls to rotation direction. The rotation speed is not controlled.


I/O table of L298N.


E1/2 	M1/2		Motor
0 		1		rotate forward
1 		0		rotate backward
0 		0			stop


The E1/2 pin controls if the motor rotates or not. 1 means rotate. 0 meas stop.
The M1/2 pin controls the rotation direction. 1 and 0 are oposite direction. It only makes sense when E1 is 1.


The circuit:
---------------------------------------------
	Arduino		L298N(Motor Driver)			
	4*				E1
	5*				M1
	6*				E2
	7*				M2

	GND				GND
	VIN(9V)			VD

The pin number with the * suffix means you can change it to be any number.


---------------------------------------------
	Motor Battery	   L298N	 Ardiono
	GND					GND		  GND
	+4.8~35V			VS		  N/A

---------------------------------------------
	Logic Battery	   L298N	 Ardiono
	GND					GND		  GND
	+7~12V				VD	  	  Vin

---------------------------------------------
	L298N		Motor
	M1			DC Motor
	M2			DC Motor
---------------------------------------------

*/

#ifndef L298N_H
#define L298N_H

#include <Arduino.h>

class L298N
{
  public:
	L298N(unsigned char e, unsigned char m, bool reverse = false);

  public:
	void forward(unsigned int speed = 1023);
	void backward(unsigned int speed = 1023);
	void stop(unsigned int speed = 1023, bool slow = false);

  private:
	unsigned char m_e;
	unsigned char m_m;
	bool m_reverse;
};
#endif // L298N_H
