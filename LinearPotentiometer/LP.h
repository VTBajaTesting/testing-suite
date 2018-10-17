#include<iostream>
#include<thread>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<unistd.h>
#include<fcntl.h>
#include <signal.h>
#include<sstream>


using namespace std;

/*
Michael Bock - 10/9/2018
This is the object that reads from the linear potentiometer(s) for the VT 2019 Baja Car

*/
#ifndef LIN_POT
#define LIN_POT
class LinPot{
        private:
                int val;
                double m;
                double b;
                int adcPort;
		//reads ADC file for linear potentiometer
		int linearPotent();

	public:
                LinPot(double slope, double intercept, int value, int adcPort);
		LinPot();
		//returns linearPotentiometer value
                int getVal();

};
#endif

