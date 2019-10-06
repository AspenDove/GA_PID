#pragma once
#include "com.h"


class motor
{
public:
	typedef struct
	{
		int16_t speed;
		float pid[3];
	}data_pack;
	float Kp = 0, Ti = 0, Td = 0;

	motor()
	{
		char COM[20] = "COM4";

		OpenPort(COM);
		SetupDCB(115200, hComm);  //这里后期可以设置串口波特率
		SetupTimeout(0, 0, 0, 0, 0);    //不设置读、写超时
		PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  //清空串口1缓冲区
		reset();
	}
	rec recParam;
	void getParam(void)
	{
		recParam = ReciveString(hComm);
	}
	void setParam(int16_t speed,float Kp,float Ti,float Td)
	{
		data_pack data{speed,Kp,Ti,Td};
		WriteHEX((CHAR*)(&data), sizeof(data_pack), hComm);
	}
	void reset()
	{
		setParam(0, 0, 0, 0);
		do 
		{
			Sleep(2);
			getParam();
		} while (!(recParam.iSpeed <= 22 || recParam.iSpeed >= 8170));//!(recParam.iSpeed <= 22 || recParam.iSpeed>=8170)recParam.iSpeed>=1
		cout << "reset\n";
		return;
	}

	bool update(float Kp, float Ti, float Td)
	{
		static int16_t adjspeed = 1000;
		getParam();
		
		
		float error = abs(recParam.iSpeed - adjspeed);
		integrate += error;
		
		
		if (error < 100)
		{
			if (adjspeed == 1000)adjspeed = 4000;
			else adjspeed = 1000;
		}
		setParam(adjspeed, Kp, Ti, Td);
		Sleep(2);
		
		
		
		if (integrate > 1000000)
		{
			adjspeed = 1000;
			integrate = 0;
			return false;
		}
		else return true;
	}
	float integrate = 0;
};