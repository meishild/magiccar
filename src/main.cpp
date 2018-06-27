/*
Version 1.0:根据命令(串口通信或者蓝牙通信)能控制小车执行前进、后退、停止、左转、右转。
Depend:+9V电源、小车车架、轮胎以及电机、l298n控制板、蓝牙从机、arduino板子。

Control Commands (They are remote commands. So add the router prefix "0x50 0x01 0x02".)
Forward: "DR 11\r\n"
Backward: "0x71\r\n"
Turn right: "0x72\r\n"
Turn Left: "0x73\r\n"
Stop: "0x74\r\n"

Command flow
PC --> UART --> ERxTextMessage --> UARTCmdReceiverService --> L298NMotorService 

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
#include <SerialCommands.h>

#define DEBUG_SERIAL Serial
#define UART_SERIAL Serial3

char serial_command_buffer[128];
SerialCommands serial_commands(&UART_SERIAL, serial_command_buffer, sizeof(serial_command_buffer), "\r\n", " ");

//-----------------------------------------------------------------------------------------
// Define the host and the supported services
L298NMotorService motorService;

#define E1 2 // Left motor
#define M1 3
#define E2 4 // Right motor
#define M2 5
#define E3 6 // Right motor
#define M3 7
#define E4 8 // Left motor
#define M4 9

void cmd_unrecognized(SerialCommands *sender, const char *cmd)
{
	DEBUG_SERIAL.print("ERR:UNKNOW CMD[");
	DEBUG_SERIAL.print(cmd);
	DEBUG_SERIAL.println("]");
}

void cmd_driver(SerialCommands *sender)
{
	char *dcStr = sender->Next();
	if (dcStr == NULL)
	{
		DEBUG_SERIAL.println("ERR:NO DRIVE CMD");
		return;
	}
	int commandId = atoi(dcStr);
	bool res = motorService.execute(commandId);

	DEBUG_SERIAL.print("CMD:DRIVE->");
	DEBUG_SERIAL.print(commandId);
	DEBUG_SERIAL.print(",");
	DEBUG_SERIAL.println(res ? "SUCCESS" : "FAIL");
}

void cmd_connected(SerialCommands *sender){
	DEBUG_SERIAL.println("CMD:UART->CONNECTED");
}

void cmd_discon(SerialCommands *sender){
	DEBUG_SERIAL.println("CMD:UART->DISCON");
}

//Note: Commands are case sensitive
SerialCommand cmdDriver("DR", cmd_driver);
SerialCommand cmdConnected("CONNECTED\r", cmd_connected);
SerialCommand cmdDiscon("+DISC:SUCCESS\r", cmd_discon);

void setup()
{
	DEBUG_SERIAL.begin(9600);
	UART_SERIAL.begin(9600);

	motorService.addLeftMotor(E1, M1);
	motorService.addLeftMotor(E4, M4);
	motorService.addRightMotor(E2, M2);
	motorService.addRightMotor(E3, M3);

	serial_commands.SetDefaultHandler(cmd_unrecognized);
	serial_commands.AddCommand(&cmdDriver);
	serial_commands.AddCommand(&cmdConnected);
	serial_commands.AddCommand(&cmdDiscon);

	DEBUG_SERIAL.println("System startup!");
}

void loop()
{
	serial_commands.ReadSerial();
}
