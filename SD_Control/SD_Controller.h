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
	Accel accelerometerR=Accel((const char*)"1c",8);
	Accel accelerometerL=Accel((const char*)"1d",8);
	string my_filename;
	short    my_updateNum;
	long   my_systemUpTime;
	short    my_numOfLinPot;
	short    my_numOfAccel;
	short    my_numOfIMU;
	short    my_otherShit;
	string extended_Filename;
	LinPot fRPotentiometer;
	LinPot fLPotentiometer;
	LinPot bRPotentiometer;
	LinPot bLPotentiometer;
	IMU* imu=new IMU();
public:
	SD_Controller(string _filename, short  _updateNum, long _systemUpTime, short _numOfLinPot,
		short _numOfAccel, short _numOfIMU, short _otherShit);

	bool open_File();
	bool close_File();
	bool write_Data(int time);



};


#endif // !SD_Cont
