#include "IMU.h"
#include "IMU.cpp"

int main()
{
	IMU* imu=new IMU();
	//std::cout<<"IMU Created"<<std::endl;
	std::cout<<imu->getAccelX()<<std::endl;
	std::cout<<imu->getAccelY()<<std::endl;
	std::cout<<imu->getAccelZ()<<std::endl;
	std::cout<<imu->getGyroX()<<std::endl;
	std::cout<<imu->getGyroY()<<std::endl;
	std::cout<<imu->getGyroZ()<<std::endl;
	return 0;
}
