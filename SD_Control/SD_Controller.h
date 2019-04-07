#pragma once
#include "../LinearPotentiometer/LP.h"
#include "../Accelerometer/accel.h"
#include "../IMU/IMU.h"
#include "../GPS/GPS.h"
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <string.h>

#include <string>
#include <vector>
//#include <string.h>
//#include <cstring>
#include "../sensors/I2CSensor.h"
#include "../sensors/Sensor.h"


#ifndef SD_Cont // SD_Cont
#define SD_Cont
using namespace std;
class SD_Controller {
	//std::string test = "";

private:
	string file_Location = "";
	string my_filename;
	string extended_Filename;
	long system_up_time;
	ofstream myFile;
	ofstream gpsFile;
	//Accel accelerometerR=Accel((const char*)"1c",8);
	//Accel accelerometerL=Accel((const char*)"1d",8);
	std::vector<Sensor*> sensors;
	std::vector<I2CSensor*> i2csensors;
	GPS* gps;
public:
	SD_Controller(string _filename, long _systemUpTime,std::vector<Sensor*> s,std::vector<I2CSensor*>i);

	bool open_File();
	bool close_File();
	bool write_Data(int time);
	void write_GPS();
};


#endif // !SD_Cont
