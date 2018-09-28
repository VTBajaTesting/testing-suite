#include<iostream>
#include<thread>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<unistd.h>
#include<fcntl.h>
#include <signal.h>
#include<sstream>
//#include "LP.h"
volatile sig_atomic_t stop;

using namespace std;
void inthand(int signum){
        stop=1;
}
/*
This is the object that reads from the linear potentiometer(s) for the VT 2019 Baja Car

*/
#ifndef LP
#define LP
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

		//returns linearPotentiometer value
                int getVal();

};
#endif

