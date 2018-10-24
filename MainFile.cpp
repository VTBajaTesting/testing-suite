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
//User-Designed Libraries
#include "Accelerometer/accel.h"
#include "LinearPotentiometer/LP.h"
#include "SD_Control/SD_Controller.h"
#include "SD_Control/SD_Controller.cpp"
#include "LinearPotentiometer/LP.cpp"
#include "Accelerometer/accel.cpp"
//using namespace std;

int main()
{
	std::cout << "Hello again World!" << std::endl;

	//string temp = "";
	//cin >> temp;
	std::string name="chooch";
	SD_Controller sd(name, 1, 2048, 1, 1, 1, 1);
	for(int i=0;i<10;i++)
	{
		sd.write_Data("");
	}
	//return;
	return 0;
}

