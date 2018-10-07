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
#include <chrono>
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
int main(int argc,char* argv[])
{
	int numSamples=0;
	double totalTime=0;
	int temp = 1;
	//std::cout << "Switch:" << monitorSwitch() << std::endl;
	std::string name="files/TS1atTime";
	//Note: we should really find a better way to make the line below better
	//the way we should do it is to put this thread to sleep
	// and wake it when we want to read. This will do for now though
	while(monitorSwitch()==0){
		std::cout<<"Waiting for first read..."<<std::endl;
		system("clear");
	}//waits for first file to be created to start
	system("rm /home/debian/testing-suite/files/*");	
	auto beginning=chrono::steady_clock::now();
	while(1==1){
		clock_t newFileTime=clock();
		//puts new csv files in the directory files/<filename>.csv
		//File timestamps are in microseconds.
		//note:don't pull power while the sensors are being read.
		//     Always flip the switch back to the 
		//     off position to before pulling power to
		//     avoid messing up readings.
		//all timestamps are in milliseconds
		SD_Controller sd=SD_Controller(name,chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-beginning).count(), 2048, 1, 1, 1, 1);
		//std::cout<<((double)newFileTime-start)/CLOCKS_PER_SEC<<std::endl;
		auto startTest=chrono::steady_clock::now();
		while(monitorSwitch()==1){
			numSamples++;
			sd.write_Data(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-beginning).count());
		}
		auto end=chrono::steady_clock::now();
		usleep(15000);
		if(numSamples > 0){
		std::cout<<"Read "<<numSamples<<" samples in "<<chrono::duration_cast<chrono::milliseconds>(end - startTest).count()/1000.0<<" seconds"<<std::endl;
		std::cout<<"Speed: "<<numSamples/(chrono::duration_cast<chrono::milliseconds>(end - startTest).count()/1000.0)<<" Hz"<<std::endl;}
		temp = numSamples;
		numSamples=0;
		totalTime=0;
		//std::cout<<"New File Starting"<<std::endl;
		sd.close_File();
		while(monitorSwitch()==0){
		}
	}

	return 0;
}

