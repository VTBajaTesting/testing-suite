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
#include <mutex>
//User-Designed Libraries
#include "Accelerometer/accel.h"
#include "LinearPotentiometer/LP.h"
#include "Timer/timer.h"
#include "LinearPotentiometer/LP.cpp"
#include "Accelerometer/accel.cpp"
#include "IMU/IMU.cpp"
#include "Timer/timer.cpp"
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
	//Note: we should really find a better way to make the line below better
	//the way we should do it is to put this thread to sleep
	// and wake it when we want to read. This will do for now though
	while(monitorSwitch()==0){
		std::cout<<"Waiting for first read..."<<std::endl;
		system("clear");
	}//waits for first file to be created to start
	system("rm files/*");
	IMU imu( 0x68, 2 );//do this in the constructor unless c++ whines
	LinPot lp0(-1, 0);
	LinPot lp1(-1, 1);
	LinPot lp2(-1, 2);
	LinPot lp3(-1, 3);
	Timer timer;
	auto s = chrono::steady_clock::now();
	while(1==1)
	{
		std::vector<std::thread> threads;
		auto beg = std::chrono::steady_clock::now();	
		std::thread imuThread(&IMU::readImu, &imu);
		std::thread lp0Thread(&LinPot::readLP, &lp0);
		std::thread lp1Thread(&LinPot::readLP, &lp1);
		std::thread lp2Thread(&LinPot::readLP, &lp2);
		std::thread lp3Thread(&LinPot::readLP, &lp3);
		std::thread timerThread(&Timer::readTimer, &timer);
		threads.push_back(move(imuThread));
		threads.push_back(move(lp0Thread));
		threads.push_back(move(lp1Thread));
		threads.push_back(move(lp2Thread));
		threads.push_back(move(lp3Thread));
		threads.push_back(move(timerThread));
		for(int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}
		auto end = std::chrono::steady_clock::now();
		//write all the sensor's readings to file
		std::ofstream file("files/TS1atTime"+to_string(chrono::duration_cast<chrono::milliseconds>(end-s).count()/1000.0)+".csv", std::ofstream::out);
		std::vector<std::vector<int>> imuReadings = imu.getReadings();
		std::vector<int> lp0Readings = lp0.getReadings();
		std::vector<int> lp1Readings = lp1.getReadings();
		std::vector<int> lp2Readings = lp2.getReadings();
		std::vector<int> lp3Readings = lp3.getReadings();
		std::vector<int> timerReadings = timer.getReadings();
		std::vector<std::vector<int>> finalReadings;
		unsigned int sizes[6] = {timerReadings.size(),lp0Readings.size(), lp1Readings.size(), lp2Readings.size(), lp3Readings.size(), imuReadings.size()};
		unsigned int maxSize = 0;
		for(int i = 0; i < 6; i++)
		{
			if(sizes[i] > maxSize)
			{
				maxSize = sizes[i];
			}
			}
		std::cout << "Writing Data" << std::endl;
		for(int i = 0; i < maxSize; i++)
		{
			std::cout << "Wrote" << std::endl;
			if(i < sizes[0])
			{
				file<<timerReadings[i]<<",";
			}
			else
			{
				file<<"-1000,";
			}
			if(i < sizes[1])
			{
				file<<lp0Readings[i]<<",";
			}
			else
			{
					file<<"-1000,";
			}
			if(i < sizes[2])
			{
				file<<lp1Readings[i]<<",";
			}
			else
			{
				file<<"-1000,";
			}
			if(i < sizes[3])
			{
				file<<lp2Readings[i]<<",";
			}
			else
			{
				file<<"-1000,";
			}
			if(i < sizes[4])
			{
				file<<lp3Readings[i]<<",";
			}
			else	
			{	
				file<<"-1000,";
			}
			if(i < sizes[5])
			{
				file<<imuReadings[i][0]<<","<<imuReadings[i][1]<<","<<imuReadings[i][2]<<","<<imuReadings[i][3]<<","<<imuReadings[i][4]<<","<<imuReadings[i][5]<<","<<imuReadings[i][6];
			}
			else
			{
				file<<"-1000,-1000,-1000,-1000,-1000,-1000";
			}
			file<<"\n";
		}
		file.close();	
		std::cout<<"Got "<<maxSize<<" samples in "<<std::chrono::duration_cast<chrono::milliseconds>(end - beg).count()/1000.0<<"s"<<std::endl;
		std::cout<<"Speed = "<<maxSize/(chrono::duration_cast<chrono::milliseconds>(end - beg).count()/1000.0)<<" Hz"<<std::endl;
		while(monitorSwitch() == 0){}
	}
	return 0;
}

