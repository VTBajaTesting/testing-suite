#include "IMU.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stropts.h>
#include <stdio.h>
#include "../Switch/switch.h"
#include "../mutex.h"

extern std::mutex m;
IMU::IMU( const int address, const int bus){
        	
	this->address = address;
	this->bus = bus;
	char imu_name[64];
        snprintf(imu_name, sizeof(imu_name), "/dev/i2c-%d", bus);
        //open the i2c bus
	if((file = open(imu_name, O_RDWR)) < 0)
	{
		std::cout << "Failed to open i2c bus to read from IMU." << std::endl;
	}	

	//access the IMU
	if(ioctl(file, I2C_SLAVE, address) < 0)
	{
		printf("Failed to access IMU Slave Device");
	}


}
void IMU::readImu()
{

        //send start condition
	char imu_name[64];
        snprintf(imu_name, sizeof(imu_name), "/dev/i2c-%d", bus);
	int s = 1;
	while(s == 1)
	{
        	//open the i2c bus
		if((file = open(imu_name, O_RDWR)) < 0)
		{
			std::cout << "Failed to open i2c bus to read from IMU." << std::endl;
		}	

		//access the IMU
		if(ioctl(file, I2C_SLAVE, address) < 0)
		{
			printf("Failed to access IMU Slave Device");
		}
		//indicate to the IMU that we want data
		char buf[1] = { 0x00 };
		if(write(file, buf, 1) != 1)
		{
			std::cout << "Failed to start data transaction with IMU(HIGH to LOW Transition failed) in IMU::read" << std::endl;
		}
		int expectedNumBytes = 0x7E;

		int numOfBytesRead = read(file, this->dataBuf, expectedNumBytes);
        	//get readings from the IMU
		if(numOfBytesRead <= 0)
		{
			std::cout << "Failed to complete data transaction with IMU" << std::endl;
		}
		close(file);
       		int gyroX,gyroY,gyroZ,accelX,accelY,accelZ,temp = 0;
		//get integers from those readings
		process(67, 68, gyroX);
        	process(69, 70, gyroY);
		process(71, 72, gyroZ);
		process(65, 66, temp);
		process(59, 60, accelX);
		process(61, 62, accelY);
		process(63, 64, accelZ);
		std::vector<int> sample(7, -1);
		sample[0] = gyroX;
		sample[1] = gyroY;
		sample[2] = gyroZ;
		sample[3] = accelX;
		sample[4] = accelY;
		sample[5] = accelZ;
		sample[6] = temp;
		readings.push_back(sample);
//		m.lock();
		s = monitorSwitch();
//		m.unlock();
	}
}

void IMU::process(int idxLow, int idxHigh, int& val){
	//do some post processing of the gyro data
	short value = dataBuf[idxHigh];//get the most significant byte of the reading
	value = (value << 16) | dataBuf[idxLow]; //shift the high bit over 8 to align it with the low bit and or this mf
	value = value >> 2; //account for 2s completement
	value = ~value + 1;
        val = value;	
}

std::vector<std::vector<int>> IMU::getReadings()
{
	return readings;
}
string IMU::exec(string cmd) {
	string data="";
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	//strcat((char*)cmd.c_str()," 2>&1");
	stream = popen((char*)cmd.c_str(), "r");
	if(!stream)
	{
		std::cout<<"IMU missing"<<std::endl;
		return "";
	}
	if (stream) {
		while (!feof(stream)){
			if (fgets(buffer, max_buffer, stream) !=NULL){
				data.append(buffer);
			}
		}
		pclose(stream);
	}
	return data;
}

/*
UNUSED FOR NOW
string IMU::getAccelX()
{
	//std::cout<<accelX<<std::endl;
	return accelX;
}
string IMU::getAccelY()
{
	return accelY;
}
string IMU::getAccelZ()
{
	return accelZ;
}

string IMU::getGyroX()
{
	return gyroX;
}
string IMU::getGyroY()
{
	return gyroY;
}
string IMU::getGyroZ()
{
	return gyroZ;
}
*/
