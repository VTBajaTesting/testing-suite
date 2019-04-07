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
#include <thread>
//User-Designed Libraries
#include "Accelerometer/accel.h"
#include "LinearPotentiometer/LP.h"
#include "SD_Control/SD_Controller.h"
#include "SD_Control/SD_Controller.cpp"
#include "LinearPotentiometer/LP.cpp"
#include "Accelerometer/accel.cpp"
#include "IMU/IMU.cpp"
#include "GPS/GPS.h"
#include "GPS/GPS.cpp"
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
	//std::cout << "Switch:" << monitorSwitch() << std::endl;
	std::string name="files/TS11atTime";
	
	//There will be no sensors except the GPS for the black box
	std::vector<I2CSensor*> i2csensors;
	//Accel* accelerometerR=new Accel((const char*)"1c",8);
	
	//i2csensors.push_back(new Accel((const char*)"1c",8));
	//i2csensors.push_back(new Accel((const char*)"1d",8));
	//i2csensors.push_back(new IMU());
	std::vector<Sensor*> sensors;
	
	//sensors.push_back(new LinPot(-1,1));
	//sensors.push_back(new LinPot(-1,0));
	//sensors.push_back(new LinPot(-1,3));
	//sensors.push_back(new LinPot(-1,2));
	//Note: we should really find a better way to make the line below better
	//the way we should do it is to put this thread to sleep
	// and wake it when we want to read. This will do for now though
	while(monitorSwitch()==0){
		std::cout<<"Waiting for first read..."<<std::endl;
		system("clear");
	}//waits for first file to be created to start
	auto beginning=chrono::steady_clock::now();
	while(1==1){
		SD_Controller sd=SD_Controller(name,chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-beginning).count(),sensors,i2csensors);
        	numSamples=0;
		totalTime=0;
		clock_t newFileTime=clock();
		//puts new csv files in the directory files/<filename>.csv
		//File timestamps are in microseconds.
		//note:don't pull power while the sensors are being read.
		//     Always flip the switch back to the 
		//     off position to before pulling power to
		//     avoid messing up readings.
		//all timestamps are in milliseconds
		//SD_Controller sd=SD_Controller(name,chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-beginning).count(), sensors, i2csensors);
		//std::cout<<((double)newFileTime-start)/CLOCKS_PER_SEC<<std::endl;
		auto startTest=chrono::steady_clock::now();
		while(monitorSwitch()==1){
			numSamples++;
			sd.write_Data(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-beginning).count());
		}
		auto end=chrono::steady_clock::now();
		std::cout<<"Read "<<numSamples<<" samples in "<<chrono::duration_cast<chrono::milliseconds>(end - startTest).count()/1000.0<<" seconds"<<std::endl;
		std::cout<<"Speed: "<<numSamples/(chrono::duration_cast<chrono::milliseconds>(end - startTest).count()/1000.0)<<" Hz"<<std::endl;
		//std::cout<<"New File Starting"<<std::endl;
		sd.close_File();
		usleep(15000);
		while(monitorSwitch()==0){}
		std::cout<<chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - beginning).count()/1000.0<<std::endl;
	}
	return 0;
}

