/*
 * StrainGauge.h
 *
 *  Created on: Oct 1, 2019
 *      Author: Anna Herms
 */

#ifndef STRAINGAUGE_H_
#define STRAINGAUGE_H_
constexpr auto E_SLAVE_COMM_FAIL = -1;
constexpr auto E_READ_FAIL = -2;

class StrainGauge {
private:
	int address;
	int bus;
	char fileName[64];
	int fileHandle;

	//There are 19 bytes total with 8 strain gauges
	//each strain gauge has two bytes
	//status, command, and id register are one byte each
	static const int STRAIN_GAUGE_COUNT = 8;

	//Slave no longer sending status and command registers (only sends  strain + id registers)
	static const int REGISTER_BYTES = (STRAIN_GAUGE_COUNT*2) + 1;
	unsigned char dataBuf[REGISTER_BYTES];
	unsigned short strainArr[STRAIN_GAUGE_COUNT];

public:
	StrainGauge(int, int);
	int readStrain();
	void storeData(int, int);
	void printRegister();
	unsigned short getStrain(int strainNum);
};

#endif /* STRAINGAUGE_H_ */
