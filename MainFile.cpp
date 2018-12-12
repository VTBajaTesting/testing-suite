// Created by: Corwin Warner - 10/3/2018
// Completed by:_____________ - 
//
//
// This is the main file for the BeagleBone Telemetry system designed for the VT Baja cars.
// 
//
//
//
//
//
//
//
//
//
//
//
//
#define _GLIBCXX_USE_CXX11_ABI 0
//Standard Libraries
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
//User-Designed Libraries
#include "Accelerometer/accel.h"
#include "LinearPotentiometer/LP.h"
#include "SD_Control/SD_Controller.h"
#include "SD_Control/SD_Controller.cpp"
#include "LinearPotentiometer/LP.cpp"
#include "Accelerometer/accel.cpp"
#include "IMU/IMU.cpp"
//using namespace std;
int monitorSwitch()
{
	stringstream fileName;
	fileName<<"/sys/class/gpio/gpio48/value";
	FILE *file=fopen(fileName.str().c_str(),"r");
        if(!file)
        {
        	perror("File Switch ");
                return -1;
        }
        rewind(file);

	//read 1 value from teh gpio pin
        char* buffer;
	buffer=(char*)malloc(sizeof(char));
        size_t reading=fread(buffer,1,1,file);
	//close the file
        fclose(file);
	//std::cout<<"Done monitoring"<<std::endl;
	return atoi(buffer);
}
int main()
{
	std::cout << "Hello again World!" << std::endl;
	int t=0;
	std::string name="files/chooch";
	clock_t start=clock();
	while(1==1){
		clock_t newFileTime=clock();
		//puts new csv files in the directory files/<filename>.csv
		//File timestamps are in microseconds.
		//note:don't pull power while the sensors are being read.
		//     Always flip the switch back to the 
		//     off position to before pulling power to
		//     avoid messing up readings.
		SD_Controller sd=SD_Controller(name,((double)(newFileTime-start))/CLOCKS_PER_SEC*1000000, 2048, 1, 1, 1, 1);
		std::cout<<((double)newFileTime-start)/CLOCKS_PER_SEC<<std::endl;
		while(monitorSwitch()==0){
			sd.write_Data("");
		}
		
		std::cout<<"New file"<<std::endl;
		sd.close_File();
		usleep(300);
		while(monitorSwitch()==1){}
	}
	return 0;
}

