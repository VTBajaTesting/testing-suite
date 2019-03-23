#include "IMU.h"
IMU::IMU(){
	std::cout<<"A"<<std::endl;
	//data="";
	//exec("i2cset -y 2 0x68 0x6B 0");	
	std::cout<<"B"<<std::endl;
	accelX="";
	accelY="";
	accelZ="";
	gyroX="";
	gyroY="";
	gyroZ="";
}
/*IMU::~IMU()
{
std::cout<<"Destroying IMU"<<std::endl;
delete &accelX;
delete &accelY;
delete &accelZ;
delete &gyroX;
delete &gyroY;
delete &gyroZ;
}*/
string* IMU::read()
{
	string gyroX=get(devAddr,gyroXH).substr(2,2) + get(devAddr,gyroXL).substr(2,2);
	string gyroY=get(devAddr,gyroYH).substr(2,2) + get(devAddr,gyroYL).substr(2,2);
	string gyroZ=get(devAddr,gyroZH).substr(2,2) + get(devAddr,gyroZL).substr(2,2);
	string* accel=accelerometer.read();
	static string readings[6]={*(accel),*(accel+1),*(accel+2),gyroX,gyroY,gyroZ};
	return readings;
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
