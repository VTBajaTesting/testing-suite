#include<iostream>
#include<thread>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<unistd.h>
#include<fcntl.h>
#include <signal.h>
#include<sstream>
#include <vector>
#include "../Switch/switch.h"
/*
Michael Bock - 10/9/2018
This is the object that reads from the linear potentiometer(s) for the VT 2019 Baja Car

*/
#ifndef LIN_POT
#define LIN_POT
class LinPot{
        private:
                short val;
                short adcPort;
		std::vector<int> readings;
	public:
                LinPot(short value, short adcPort);
		LinPot();
		std::vector<int> getReadings();
		//returns linearPotentiometer value
                void readLP();

};
#endif

