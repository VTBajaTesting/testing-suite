#pragma once

#include <iostream>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#include <string.h>

//#include <string>
//#include <string.h>
//#include <cstring>

using namespace std;

#ifndef SD_Cont // SD_Cont
#define SD_Cont

class SD_Controller {
	//std::string test = "";

private:
	string file_Location = "";

	ofstream myFile;
	
	string my_filename;
	int    my_updateNum;
	long   my_systemUpTime;
	int    my_numOfLinPot;
	int    my_numOfAccel;
	int    my_numOfIMU;
	int    my_otherShit;
	string extended_Filename;
	

public:
	SD_Controller(string _filename, int _updateNum, long _systemUpTime, int _numOfLinPot,
		int _numOfAccel, int _numOfIMU, int _otherShit);

	bool open_File();
	bool close_File();
	bool write_Data(string _information);



};


#endif // !SD_Cont
