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
#define bufSize 0x7E
using namespace std;
class IMU{
	private:
		//Note: this code needs some cleaning
		int gyroX;//TODO:gotta initialize these, for now we can say they are undefined until they are read in, which actually kinda makes sense
		int gyroY;
		int gyroZ;
                int file;
		int temp;
		int accelX;
		int accelY;
		int accelZ;
		char dataBuf[bufSize];
		int address;
		int bus;
		//string data;
		//executes i2cget commands to return readings
		string exec(string cmd);
		void process(int idxLow, int idxHigh, int& val);
	public:
		IMU( const int address, const int bus);
		~IMU();	
		void readImu();

		int getGyroX();
		int getGyroY();
		int getGyroZ();
		int getAccelX();
		int getAccelY();
		int getAccelZ();
		int getTemp();
		
};
#endif
