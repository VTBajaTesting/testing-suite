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
//Standard Libraries
#include <stdio.h>
#include <iostream>
#include <fstream>
//User-Designed Libraries
#include <accel.h>
#include <LP.h>
#include <SD_Controller.h>


using namespace std;

int main(void)
{
	cout << "Hello again World!" << endl;

	//string temp = "";
	//cin >> temp;

	SD_Controller::SD_Controller("chooch", 1, 2048, 1, 1, 1, 1);
	SD

	//return;

}

