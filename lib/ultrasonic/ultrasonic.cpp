#include <ultrasonic.h>

Ultrasoinc::Ultrasoinc(unsigned char trigPin, unsigned char echoPin, unsigned char filterSize)
{
    _trigPin = trigPin;
    _echoPin = echoPin;
    _filterSize = filterSize;
    pinMode(_trigPin, INPUT);
    digitalWrite(_echoPin, OUTPUT);
}

unsigned char Ultrasoinc::read()
{
    float filter_buf[_filterSize];

   
    for (int i = 0; i < _filterSize; i++)
    {
        digitalWrite(_trigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
        delayMicroseconds(2);
        digitalWrite(_trigPin, HIGH);
        delayMicroseconds(20);
        digitalWrite(_trigPin, LOW);
        filter_buf[i] = pulseIn(_echoPin, HIGH) * 17 / 1000;
    }

    // 采样值从小到大排列（冒泡法）
    for (int j = 0; j < _filterSize - 1; j++)
    {
        for (int i = 0; i < _filterSize - 1 - j; i++)
        {
            if (filter_buf[i] > filter_buf[i + 1])
            {
                float filter_temp = filter_buf[i];
                filter_buf[i] = filter_buf[i + 1];
                filter_buf[i + 1] = filter_temp;
            }
        }
    }
    // 取中间值
    float middle = filter_buf[(_filterSize - 1) / 2];
    float FILTER_OFFSET = middle * 0.2;
    float sum = 0;

    // 滤波，如果小于如果和中位数偏差超过FILTER_OFFSET则修改值为中位数。
    for (int i = 0; i < _filterSize; i++)
    {
        int value = filter_buf[_filterSize];
        if ((value - middle) > FILTER_OFFSET || (middle - value) > FILTER_OFFSET)
        {
            sum += middle;
        }
        else
        {
            sum += value;
        }
    }

    distance = sum / _filterSize;
    return OK;
}