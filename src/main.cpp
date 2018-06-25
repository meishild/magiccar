/*
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

#include <ERxL298N.h>
#include <ERxL298NMotorService.h>
#include <ERxTextMessage.h>
#include <ERxHost.h>
#include <ERxSystemService.h>
#include <ERxMessageRouterService.h>
#include <ERxUARTCmdReceiverService.h>

//--------------CONFIGURATION-------------------------------------------------------------
// Macro definitions which require modifying for different harwares.
#define MY_ADDRESS 0x01
// ***IMPORTANT*** :Set true on the coordinator, and false on end device.
#define ROUTER_RELAY_ENABLE true

//-----------------------------------------------------------------------------------------

// Allocate the buffers
#define COMMAND_PARAMETER_BUFFER_SIZE 128
#define RESULT_BUFFER_SIZE 128
static uint8_t sCmdBuffer[COMMAND_PARAMETER_BUFFER_SIZE];
static uint8_t sResultBuffer[RESULT_BUFFER_SIZE];

// XBee
// AES encryption dictates that the maximum RF packet size is 95 Bytes.
// The XBee buffer size is 95. To make sure the message sent to XBee is packed into a package, this buffer should be less than 95.
#define ROUTER_BUFFER_SIZE 90
static uint8_t sRouterBuffer[ROUTER_BUFFER_SIZE];

// Define the host and the supported services
ERxHost host(sCmdBuffer, COMMAND_PARAMETER_BUFFER_SIZE, sResultBuffer, RESULT_BUFFER_SIZE);
ERxSystemService sysService(&host);
ERxUARTCmdReceiverService uartService(&Serial);
ERxMessageRouterService routerService(&host, &Serial, sRouterBuffer, ROUTER_BUFFER_SIZE);
ERxL298NMotorService motorService;

#define E1 2 // Left motor
#define M1 3
#define E2 4 // Right motor
#define M2 5
#define E3 6 // Right motor
#define M3 7
#define E4 8 // Left motor
#define M4 8

void setup()
{
	Serial.begin(9600);

	// Set the address.
	host.SetMyAddress(MY_ADDRESS);
	routerService.SetRelayMessage(ROUTER_RELAY_ENABLE); // IMPORTANT: Only set true on the coordinator

	// Add service to host.
	host.AddService(&sysService);
	host.AddService(&uartService);
	host.AddService(&routerService);
	host.AddService(&motorService);
	motorService.addLeftMotor(E1, M1);
	motorService.addLeftMotor(E4, M4);
	motorService.addRightMotor(E2, M2);
	motorService.addRightMotor(E3, M3);

	Serial.println("System startup!");
}

void loop()
{
	host.Run();
	Serial.println("System ready!");
}
