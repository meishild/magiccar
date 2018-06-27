#ifndef _ULTRASOINC_H
#define _ULTRASOINC_H

#include <Arduino.h>

#define OK 1
#define ERROR 0

/**
 * 读取一个引脚的脉冲（HIGH或LOW）。
 * 例如，如果value是HIGH，pulseIn()会等待引脚变为HIGH，
 * 开始计时，再等待引脚变为LOW并停止计时。
 * 
 * 返回脉冲的长度，单位微秒。如果在指定的时间内无脉冲函数返回。
 * 此函数的计时功能由经验决定，长时间的脉冲计时可能会出错。
 * 计时范围从10微秒至3分钟。（1秒=1000毫秒=1000000微秒）
 * 接收到的高电平的时间（us）* 340m/s / 2 = 
 *    接收到高电平的时间（us） * 17000 cm / 1000000 us = 
 *    接收到高电平的时间 * 17 / 1000  (cm)
 **/

//最远测量距离，单位厘米
#define MAX_DISTANCE 40
//超时时间单位微秒 MAX_DISTANCE * 1000 / 17
#define MAX_TIMEOUT 2000
class Ultrasoinc
{
public:
  Ultrasoinc(unsigned char trigPin, unsigned char echoPin, unsigned char filterSize = 10);
  unsigned char read();
  float distance;

private:
  unsigned char _trigPin;
  unsigned char _echoPin;
  unsigned char _filterSize;
};

#endif