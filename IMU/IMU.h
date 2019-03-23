/*
Michael Bock
this is a class to read from the accelerometer and gyroscope in an IMU

----------------------------------------------------------------------
Gyro Mode: +250 dps
Accelerometer Mode: +2g
*/
#include<cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <memory>
#include <array>

#include "../sensors/I2CSensor.h"
#include "../Accelerometer/accel.h"
#ifndef IMEASU
#define IMEASU

using namespace std;
class IMU:public I2CSensor
{
	private:
		string accelX;
		string accelY;
		string accelZ;
		string gyroX;
		string gyroY;
		string gyroZ;
		char* devAddr=" 0x68";
		Accel accelerometer=Accel((const char*)"59",2);
		const char* gyroXH=" 0x43";
		const char* gyroXL=" 0x44";
		const char* gyroYH=" 0x45";
		const char* gyroYL=" 0x46";
		const char* gyroZH=" 0x47";
		const char* gyroZL=" 0x48";
	public:
		IMU();
		~IMU();	
		string* read();
		//reads acceleromtere X value
		/*string getAccelX();
		
		//reads accelerometer Y value
		string getAccelY();

		//reads acceleromter Z value
		string getAccelZ();

		//reads gyro X value
		string getGyroX();

		//reads gyro Y value
		string getGyroY();

		//reads gyro Z value
		string getGyroZ();*/
};
#endif
