#include <Python.h>
#include <iostream>
#ifndef GPS_MOD
#define GPS_MOD
//wrapper for GPS read
class GPS{
	public:
		GPS(){};
		char* read();
		double* parse();
};
#endif
