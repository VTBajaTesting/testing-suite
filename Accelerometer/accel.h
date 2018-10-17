#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <cstring>
#include <string>
#include <memory>
#include <array>
#include <iostream>
#include <math.h>
using namespace std;

#ifndef ACCEL
#define ACCEL
// this is the class definition
class Accel
{
// these strings hold the readings
string accel_x = "";
string accel_y = "";
string accel_z = "";
// this holds the range
int range = 8;

//default 0x1D and 0x1
//this holds the devices adress
char* deviceADDR = new char[5];
//address of the power control register
const char* PWR_Control = " 0x2A ";
//address of the xyz range control register
const char* Mode_Control = " 0x0E ";
//addresses of the acceleration readings
const char* accel_xH = " 0x01";
const char* accel_xL = " 0x02";

const char* accel_yH = " 0x03";
const char* accel_yL = " 0x04";
const char* accel_zH = " 0x05";
const char* accel_zL = " 0x06";

//command strings the need registers to work
const char* cmdGet = "i2cget -y 2";
const char* cmdSet = "i2cset -y 2";

public:

//methods : see cpp file for more info
Accel(const char*, int);
Accel();
~Accel();
string exec(char*);
string get(char*, const char*);
void set(char*, const char*, int);
void read();
string getX();
string getY();
string getZ();
int getRange();
};

#endif


//conversion function
double stringToGs( string, int);
