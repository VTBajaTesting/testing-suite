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
#ifndef IMEASU
#define IMEASU

using namespace std;
class IMU{
	private:
		string accelX;
		string accelY;
		string accelZ;
		string gyroX;
		string gyroY;
		string gyroZ;
		//string data;
		//executes i2cget commands to return readings
		string exec(string cmd);
	public:
		IMU();
		~IMU();	
		void read();
		//reads acceleromtere X value
		string getAccelX();
		
		//reads accelerometer Y value
		string getAccelY();

		//reads acceleromter Z value
		string getAccelZ();

		//reads gyro X value
		string getGyroX();

		//reads gyro Y value
		string getGyroY();

		//reads gyro Z value
		string getGyroZ();
};
#endif
