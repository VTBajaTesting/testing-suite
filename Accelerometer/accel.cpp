#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include "accel.h"
#include <iostream>
#include <math.h>

using namespace std;

#define MAX_BUS 64

#define ACC_X_LSB 	0x01
#define ACC_X_MSB 	0x02
#define ACC_Y_LSB 	0x03
#define ACC_Y_MSB 	0x04
#define ACC_Z_LSB 	0x05
#define ACC_Z_MSB 	0x06

#define RANGE	 	0x0E  //bits 1,0 -> RANGE OF READINGS
#define POWER		0x2A  //bits 1,0  -> POWER


Accel::Accel(int bus, int address) {
    I2CBus = bus;
    I2CAddress = address;
    writeByte(POWER, 1);
}

void Accel::readData(){

    char namebuf[MAX_BUS];
    snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);

    //Open i2c bus
    if ((file = open(namebuf, O_RDWR)) < 0){
        std::cout << "Failed to open accelerometer on " << namebuf << " I2C Bus" << std::endl;
    }

    //access accelerometer
    if (ioctl(file, I2C_SLAVE, I2CAddress) < 0){
        std::cout << "I2C_SLAVE address " << I2CAddress << " failed..." << std::endl;
    }

    //start transmission
    char buf[1] = {0x3B};
    if(write(file, buf, 1)!=1){
    	std::cout << "Failed to start data transmission with Accelerometer" << endl;
    }

    //read device state into dataBuffer
    int numberBytes = 0x7F;
    int bytesRead = read(file, this->dataBuffer, numberBytes);
    if (bytesRead <= 0){
    	std::cout << "Did not read in any accelerometer data" << std::endl;
    }
    close(file);

    this->accelerationX = convertAcceleration(ACC_X_LSB, ACC_X_MSB);
    this->accelerationY = convertAcceleration(ACC_Y_LSB, ACC_Y_MSB);
    this->accelerationZ = convertAcceleration(ACC_Z_LSB, ACC_Z_MSB);
}

int Accel::convertAcceleration(int lsb_reg_addr, int msb_reg_addr){
    int16_t temp = dataBuffer[msb_reg_addr];
    temp = temp>>8;
    temp += dataBuffer[lsb_reg_addr];
    temp = temp<<4;
    if (temp < 2048) {
    	temp = ~temp + 1;
    }
    else {
	temp -= 2048;
    }
    return temp;
}

int Accel::getRange(){
    this->readData();
    char temp = dataBuffer[RANGE];
    temp = temp & 0b00011111;
    temp = temp>>1;
    this->range = (int) temp;
    return this->range;
}

/**
 * 0 (-2g to 2g)
 * 1 (-4g to 4g)
 * 2 (-8g to 8g)
 */ 
void Accel::setRange(int range){
    this->readData();
    char current = dataBuffer[RANGE];
    char temp = range << 1; //move value into bits 3,2,1
    current = current & 0b11100000; //clear the current bits 3,2,1,0
    temp = current | temp;
    if (this->writeByte(RANGE, temp)==0){
        std::cout << "Failure to update range value" << std::endl;
    }
    std::cout << "Wrote the Range Value: " << temp  << " to " << RANGE << std::endl;
}

/**
 * Returns: 1 if successful, 0 if not
 */
int Accel::writeByte(char address, char value){
    char namebuf[MAX_BUS];
    snprintf(namebuf, sizeof(namebuf), "/dev/i2c-%d", I2CBus);
    int file;
    if ((file = open(namebuf, O_RDWR)) < 0){
        std::cout << "Failed to open accelerometer  on " << namebuf << " I2C Bus" << std::endl;
	return 0;
    }
    if (ioctl(file, I2C_SLAVE, I2CAddress) < 0){
        std::cout << "I2C_SLAVE address " << I2CAddress << " failed..." << std::endl;
	return 0;
    }
    char buffer[2];
    	buffer[0] = address;
    	buffer[1] = value;
    if (write(file, buffer, 2) != 2) {
        std::cout << "Failure to write values to accelerometer." << std::endl;
	return 0;
    }
    close(file);
    return 1;
}
