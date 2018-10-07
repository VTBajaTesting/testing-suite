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
#include <thread>
#include <mutex>
#include <vector>
#include "../Switch/switch.h"
#ifndef IMEASU
#define IMEASU
#define bufSize 0x7E
using namespace std;
class IMU{
	private:
		//Note: this code needs some cleaning
                int file;
		char dataBuf[bufSize];
		int address;
		int bus;
		std::vector<std::vector<int>> readings;
		//string data;
		//executes i2cget commands to return readings
		string exec(string cmd);
		void process(int idxLow, int idxHigh, int& val);
	public:
		IMU( const int address, const int bus);
		void readImu();
		std::vector<std::vector<int>> getReadings();

};
#endif
