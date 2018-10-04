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

class Accel
{
string accel_x = "";
string accel_y = "";
string accel_z = "";
int range = 8;

//default 0x1D and 0x1
char* deviceADDR = new char[5];
const char* PWR_Control = " 0x2A ";
const char* Mode_Control = " 0x0E ";
const char* accel_xH = " 0x01";
const char* accel_xL = " 0x02";

const char* accel_yH = " 0x03";
const char* accel_yL = " 0x04";
const char* accel_zH = " 0x05";
const char* accel_zL = " 0x06";

const char* cmdGet = "i2cget -y 2";
const char* cmdSet = "i2cset -y 2";

public:


Accel(const char*, int);
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

double stringToGs( string, Accel);
