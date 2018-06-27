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

L298N reference: http://item.taobao.com/item.htm?spm=a1z09.2.9.11.fVbF4K&id=227289053&_u=4113ts22070

* Created 4 October 2014
* By Jeffrey Sun

*/

#ifndef L298N_H
#define L298N_H

#include <Arduino.h>

class L298N
{
public:
	/*
	The passed in pins will be set to be OUTPUT in the constructor.

	@constructor
	@param {unsigned char} e the pin index of Arduino which connects to the E1/2 pin of L298N.
	@param {unsigned char} m the pin index of Arduino which connects to the M1/2 pin of L298N.
	@param {bool} reverse indicate if reverse the direction.
	*/
	L298N(unsigned char e, unsigned char m, bool reverse = false);

public:
	virtual void forward(unsigned int speed = 0);
	virtual void backward(unsigned int speed = 0);
	virtual void stop();

private:
	unsigned char 	m_e; 
	unsigned char 	m_m;
	bool			m_reverse;
};
#endif // L298N_H
