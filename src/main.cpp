/*
Version 1.0:根据命令(串口通信或者蓝牙通信)能控制小车执行前进、后退、停止、左转、右转。
Depend:+9V电源、小车车架、轮胎以及电机、l298n控制板、蓝牙从机、arduino板子。

Control Commands (They are remote commands. So add the router prefix "0x50 0x01 0x02".)
Forward: "0x50 0x01 0x02 0x70\r\n"
Backward: "0x50 0x01 0x02 0x71\r\n"
Turn right: "0x50 0x01 0x02 0x72\r\n"
Turn Left: "0x50 0x01 0x02 0x73\r\n"
Stop: "0x50 0x01 0x02 0x74\r\n"
Help: "0x50 0x01 0x02 0x01\r\n"

Command flow
PC --> UART --> ERxTextMessage --> ERxUARTCmdReceiverService --> ERxMessageRouterService (override result stream) --> ERxHost.Execute() --> ERxL298NMotorService -->
PC <-- UART <-- ERxRedirectOutputStream <------------------------------------------------- (execution result)

The circuit:
---------------------------------------------
	Arduino		L298N(Motor Driver)			
	2				E1(left)
	3				M1(left)
	4				E2(right)
	5				M2(right)
	6				E3(right)
	7				M3(right)
	8				E4(left)
	9				M4(left)

	GND				GND
	VIN(9V)			VD
---------------------------------------------
	E1/2 	M1/2		Motor
	0 		1		rotate forward
	1 		0		rotate backward
	0 		0			stop

---------------------------------------------
	Circuit power	 Arduino
	GND				  GND		
	+9V				  Jack(or Vin)

---------------------------------------------
	Motor power*	  L298N	    Arduino
	GND					GND		  GND		
	+9V					VS		  N/A

The motor can use the same power with the circuit.
---------------------------------------------
	L298N		Motor
	M1			Lefter motor
	M2			Right motor
---------------------------------------------

If you want to communicate with Arduino via Bluetooth other than USB cable,
connect the bluetooth module with the wiring below. (Don't need to change the code)

Arduino 	Bluetooth
 5V			 VCC
 GND		 GND
 TX    		 RX
 RX			 TX

*/

#include <L298NMotorService.h>
#include <UARTCmdReceiverService.h>

#define DEBUG_SERIAl Serial
#define UART_SERIAL Serial

//-----------------------------------------------------------------------------------------
// Define the host and the supported services
UARTCmdReceiverService uartService(&UART_SERIAL);
L298NMotorService motorService;

#define E1 2 // Left motor
#define M1 3
#define E2 4 // Right motor
#define M2 5
#define E3 6 // Right motor
#define M3 7
#define E4 8 // Left motor
#define M4 9

void setup()
{
	DEBUG_SERIAl.begin(9600);

	motorService.addLeftMotor(E1, M1);
	motorService.addLeftMotor(E4, M4);
	motorService.addRightMotor(E2, M2);
	motorService.addRightMotor(E3, M3);

	DEBUG_SERIAl.println("System startup!");
}

void loop()
{
	uartService.populate();

	if (!uartService.hasValidCommand())
	{
		delay(10);
		return;
	}

	int commandId = uartService.getCommandId();
	uartService.invalidateCommand();

	Serial.println(commandId);

	bool res = motorService.execute(commandId);
	Serial.print(commandId);
	Serial.print(",");
	Serial.println(res);
}
