/*
Version 1.0:根据命令(串口通信或者蓝牙通信)能控制小车执行前进、后退、停止、左转、右转。
Depend:+9V电源、小车车架、轮胎以及电机、l298n控制板、蓝牙从机、arduino板子。

控制命令:

前进：   "DT 11\r\n"  "DT 110" 
后退：   "DT 12\r\n"
右转：   "DT 13\r\n"
左转：   "DT 14\r\n"
停止：   "DT 15\r\n"

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

/*
待开发：
-1.状态信息获取。
2.测速。
3.控制开关。
4.led展示。
-5.PWM调速，增加减速、加速模式。
*/

#include <L298NMotorService.h>
#include <SerialCommands.h>
#include <ultrasonic.h>
#include <TaskScheduler.h>

#define DEBUG_SERIAL Serial
#define UART_SERIAL Serial3

//-----------------------------------------------------------------------------------------
// 设置驱动轮,需要支持PWM
#define E1 2 // Left motor
#define M1 3
#define E2 4 // Right motor
#define M2 5
#define E3 6 // Right motor
#define M3 7
#define E4 8 // Left motor
#define M4 9

// 设置超声波
#define TPIN 11
#define EPIN 12

// 设置安全距离，单位cm
#define SENSOR_SAFE 10

// 设置LED
#define LED_CAR_STATUS 14
#define LED_UART_STATUS 15
#define LES_OPEN_SAFE_STOP 16

// 整体状态，0-待初始化，1-完成初始化。代码load完成状态改成1，系统异常改为-1。
int state = 0;
// 锁状态,0-已锁定无法操作，1-已解锁，锁定状态无法控制车辆。
int lockState = 0;
// 驾驶状态值,0-待机，1-前进，2-后退，3-左转，4-右转，5-停止
int driveState = 0;
// uart外部连接 0-未连接，1-已连接
int uartState = 0;

// 开关状态
bool openUltrasonic = false;
// 是否进行安全停止
bool openSafeStop = false;
// 是否使用pwd调速
bool openPwm = true;

// 数据
// pwm,直接进行最高速度
int pwmSpeed = 1023;
// 前端检测距离
float dataDistance = 0.0;
// 车速
float dataSpeed = 0.0;

//-----------------------------------------------------------------------------------------
char serial_command_buffer[128];
char *endChars[] = {"\r\n", "0"};
char *splitChars = " ";
SerialCommands serial_commands(&UART_SERIAL, serial_command_buffer, sizeof(serial_command_buffer), endChars, 2, splitChars);

L298NMotorService motorService(openPwm);
Ultrasoinc forwartUltrasoinc(TPIN, EPIN);

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
	bool res = motorService.execute(commandId, pwmSpeed);

	DEBUG_SERIAL.print("CMD:DRIVE->");
	DEBUG_SERIAL.print(commandId);
	DEBUG_SERIAL.print(",");
	DEBUG_SERIAL.println(res ? "SUCCESS" : "FAIL");
}

void cmd_uart_connected(SerialCommands *sender)
{
	DEBUG_SERIAL.println("CMD:UART->CONNECTED");
	uartState = 1;
}

void cmd_uart_discon(SerialCommands *sender)
{
	DEBUG_SERIAL.println("CMD:UART->DISCON");
	uartState = 0;
}

//Note: Commands are case sensitive
SerialCommand cmdDriver("DT", cmd_driver);
SerialCommand cmdUARTConnected("CONNECTED\r", cmd_uart_connected);
SerialCommand cmdUARTDiscon("+DISC:SUCCESS\r", cmd_uart_discon);

//-----------------------------------------------------------------------------------------

void task_read_ultrasonic()
{
	if (openUltrasonic)
	{
		if (forwartUltrasoinc.read() == ERROR)
		{
			DEBUG_SERIAL.println("SENSOR:READ ERROR");
			return;
		}
		dataDistance = forwartUltrasoinc.distance;
	}
}

bool sensor_ultrasonic_stop()
{
	//是否进行安全检查
	if (!openSafeStop)
	{
		return false;
	}

	if (dataDistance < SENSOR_SAFE)
	{
		DEBUG_SERIAL.print("SENSOR:STOP,");
		DEBUG_SERIAL.print(forwartUltrasoinc.distance);
		DEBUG_SERIAL.println("cm");
		return true;
	}
	return true;
}

//-----------------------------------------------------------------------------------------
void checkSelf()
{
	state = 1;
	if (openUltrasonic)
	{
		if (forwartUltrasoinc.read() == ERROR)
		{
			DEBUG_SERIAL.println("SENSOR:READ ERROR");
			state = -1;
		}
	}

	if (state == 1)
	{
		DEBUG_SERIAL.println("CHECK SELF:SUCCESS");
	}
	else
	{
		DEBUG_SERIAL.println("CHECK SELF:FAIL");
	}
}

void stepAddPwm()
{
	for (int i = pwmSpeed; i < 1024; i++)
	{
		pwmSpeed++;
		delay(10);
	}
}

void stepMinusPwm()
{
	for (int i = pwmSpeed; i >= 0; i--)
	{
		pwmSpeed--;
		delay(10);
	}
}

void setLedStatus()
{
	//根据状态设置所有LED
}

// ----
void setup()
{
	state = 0;
	lockState = 0;

	DEBUG_SERIAL.begin(9600);
	UART_SERIAL.begin(9600);

	motorService.addLeftMotor(E1, M1);
	motorService.addLeftMotor(E4, M4);
	motorService.addRightMotor(E2, M2);
	motorService.addRightMotor(E3, M3);
	motorService.addForwardSafeSensor(sensor_ultrasonic_stop);

	serial_commands.SetDefaultHandler(cmd_unrecognized);
	serial_commands.AddCommand(&cmdDriver);
	serial_commands.AddCommand(&cmdUARTConnected);
	serial_commands.AddCommand(&cmdUARTDiscon);

	Sch.init(); // Initialize task scheduler
	Sch.addTask(task_read_ultrasonic, 0, 100, 1);
	Sch.start(); // Start the task scheduler

	delay(1000);
	checkSelf();

	DEBUG_SERIAL.println("LOAD SETUP FINISH!");
}

void loop()
{
	if (state != 1)
	{
		delay(1000);
		checkSelf();
		return;
	}
	Sch.dispatchTasks();

	//实时获取驾驶状态
	driveState = motorService.state;

	SERIAL_COMMANDS_ERRORS err = serial_commands.ReadSerial();
	if (err == SERIAL_COMMANDS_ERROR_BUFFER_FULL)
	{
		DEBUG_SERIAL.println("ERR:FULL COMMANED BUFFER!");
		serial_commands.ClearBuffer();
	}
}
