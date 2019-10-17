/*
 * StrainGauge.cpp
 *
 *  Created on: Oct 1, 2019
 *      Author: Anna Herms
 *
 */

#include "StrainGauge.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <iostream>

using namespace std;

StrainGauge::StrainGauge(const int address, const int bus) {
	this->address = address;
	this->bus = bus;

	//Building BBB i2c device fileName from bus number
	snprintf(fileName, sizeof(fileName), "/dev/i2c-%d", bus);

	//open i2c bus
	if ((fileHandle = open(fileName, O_RDWR)) < 0) {
		cerr << "Failed to open i2c Bus" << endl;
	}
}

int StrainGauge::StrainGauge::readStrain() {

	//initiate communication
	if (ioctl(fileHandle, I2C_SLAVE, address) < 0) {
		cerr << "Failed to acquire bus access and/or talk to slave." << endl;
		return E_SLAVE_COMM_FAIL;
	}

	//Read data from i2c bus
	if (read(fileHandle, this->dataBuf, REGISTER_BYTES) != REGISTER_BYTES) {
		cerr << "Failed to read from i2c bus" << endl;
		cout << "Bytes recieved: " << dataBuf << endl;
		return E_READ_FAIL;
	}
	else {
		//cout << "Read from i2c bus." << endl;

		//Store read data into dataBuf array using storeData method

		//Register:
		//0x01 & 0x02 for strain0
		//0x03 & 0x04 for strain1
		//0x05 & 0x06 for strain2
		//0x07 & 0x08 for strain3
		//0x09 & 0x0A for strain4
		//0x0B & 0x0C for strain5
		//0x0D & 0x0E for strain6
		//0x0F & 0x10 for strain7

		//no longer sending status and command registers (id still sent)
		//i.e. strain0 at index 0 and 1 of dataBuf
		for (int i = 0; i < 8; i++) {
			storeData((i * 2), i);
		}
	}
	//close(fileHandle);
	return REGISTER_BYTES;
}

void StrainGauge::storeData(int lowIdx, int strain)
{
	//Bytes in big-endian -> MSB is at lowIdx
	unsigned short strainValue = dataBuf[lowIdx];
	//shift MSB
	strainValue <<= 8;
	//add MSB to LSB
	strainValue |= dataBuf[lowIdx + 1];
	strainArr[strain] = strainValue;
}


unsigned short StrainGauge::getStrain(int strainNum)
{
	if (strainNum < 0 || strainNum >= 8) {
		throw invalid_argument("Invalid strain gauge index.");
	}

	return strainArr[strainNum];
}

//used for debugging
void StrainGauge::printRegister()
{
	//Prints out each register byte as a short
	//NOTE: dataBuff[16] should always equal 13
	for (int i = 0; i < REGISTER_BYTES; i++) {
		cout << (short)dataBuf[i] << " " << endl;
	}
}