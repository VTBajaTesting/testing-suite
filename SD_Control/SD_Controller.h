#pragma once
#include "../LinearPotentiometer/LP.h"
#include "../Accelerometer/accel.h"
#include "../IMU/IMU.h"
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <string.h>

#include <string>
//#include <string.h>
//#include <cstring>



#ifndef SD_Cont // SD_Cont
#define SD_Cont
using namespace std;
class SD_Controller {
	//std::string test = "";

private:
	string file_Location = "";

	ofstream myFile;
	Accel accelerometerR=Accel(2,0x1c);//do this in the constructor unless c++ whines about it
	Accel accelerometerL=Accel(2,0x1d);//TODO:these are terrible variable names - should be changed to not be bad
	string my_filename;
	short    my_updateNum;//useless
	long   my_systemUpTime;//useless
	short    my_numOfLinPot;//useless
	short    my_numOfAccel;//useless
	short    my_numOfIMU;//in fact all these variables do pretty much nothing
	short    my_otherShit;//seriously, its time to remove this variable, it does nothing
	string extended_Filename;
	LinPot fRPotentiometer;//make syre these match
	LinPot fLPotentiometer;
	LinPot bRPotentiometer;
	LinPot bLPotentiometer;
	//This variable should be made in the constructor and set to 0/NULL here
	IMU* imu=new IMU( 0x68, 2 );//do this in the constructor unless c++ whines
public:
	SD_Controller(string _filename, short  _updateNum, long _systemUpTime, short _numOfLinPot,
		short _numOfAccel, short _numOfIMU, short _otherShit);//most of these arguments are usewell @corwinw

	bool open_File();
	bool close_File();
	bool write_Data(int time);//TODO: write to RAM instead



};


#endif // !SD_Cont
