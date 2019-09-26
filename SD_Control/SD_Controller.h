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
	Accel accelerometerR=Accel((const char*)"1c",8);//do this in the constructor unless c++ whines about it
	Accel accelerometerL=Accel((const char*)"1d",8);
	string my_filename;
	short    my_updateNum;
	long   my_systemUpTime;
	short    my_numOfLinPot;
	short    my_numOfAccel;
	short    my_numOfIMU;//in fact all these variables do pretty much nothing
	short    my_otherShit;//seriously, its time to remove this variable, it does nothing
	string extended_Filename;
	LinPot fRPotentiometer;
	LinPot fLPotentiometer;
	LinPot bRPotentiometer;
	LinPot bLPotentiometer;
	//This variable should be made in the constructor and set to 0/NULL here
	IMU* imu=new IMU( 0x68, 2 );
public:
	SD_Controller(string _filename, short  _updateNum, long _systemUpTime, short _numOfLinPot,
		short _numOfAccel, short _numOfIMU, short _otherShit);

	bool open_File();
	bool close_File();
	bool write_Data(int time);



};


#endif // !SD_Cont
