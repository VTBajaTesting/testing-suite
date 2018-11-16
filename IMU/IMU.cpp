#include "IMU.h"
IMU::IMU(){
	std::cout<<"A"<<std::endl;
	//data="";
	exec("i2cset -y 2 0x68 0x6B 0");	
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
void IMU::read()
{
	//string data="";
 	string getAccelH="i2cget -y 2 0x68 0x3B";
        string getAccelL="i2cget -y 2 0x68 0x3C";
        accelX = exec(getAccelH).substr(2,2)+ exec(getAccelL).substr(2,2);
	getAccelH="i2cget -y 2 0x68 0x3D";
        getAccelL="i2cget -y 2 0x68 0x3E ";
        accelY= exec(getAccelH).substr(2,2)+exec(getAccelL).substr(2,2);
	getAccelH="i2cget -y 2 0x68 0x3F";
        getAccelL="i2cget -y 2 0x68 0x40";
        accelZ=exec(getAccelH).substr(2,2)+exec(getAccelL).substr(2,2);
	string getGyroH="i2cget -y 2 0x68 0x43";
        string getGyroL="i2cget -y 2 0x68 0x44";
        gyroX=exec(getGyroH).substr(2,2)+exec(getGyroL).substr(2,2);
 	getGyroH="i2cget -y 2 0x68 0x45";
        getGyroL="i2cget -y 2 0x68 0x46";
        gyroY=exec(getGyroH).substr(2,2)+exec(getGyroL).substr(2,2);
	getGyroH="i2cget -y 2 0x68 0x47";
        getGyroL="i2cget -y 2 0x68 0x48";
        gyroZ=exec(getGyroH).substr(2,2)+exec(getGyroL).substr(2,2);

}
string IMU::exec(string cmd) {
	string data="";
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	//strcat((char*)cmd.c_str()," 2>&1");
	stream = popen((char*)cmd.c_str(), "r");
	std::cout<<cmd.c_str()<<std::endl;
	if (stream) {
		std::cout<<"Stream"<<std::endl;
		while (!feof(stream)){
			std::cout<<"Stram2"<<std::endl;
			if (fgets(buffer, max_buffer, stream) !=NULL){
				std::cout<<"buffer"<<std::endl;
				data.append(buffer);
			}
		}
		pclose(stream);
	}
	return data;
}

string IMU::getAccelX()
{
	std::cout<<accelX<<std::endl;
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
