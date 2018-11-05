#include "IMU.h"
IMU::IMU(){
	char str[100];
	string str2;
	strcpy(str,"i2cset -y 2 0x68 0x6B 0");
	strcat(str,to_string(0).c_str());
	string data;
	FILE * stream;
	const int max_buffer = 256;
 	char buffer[max_buffer];
 	strcat((char*)str," 2>&1");
 	stream = popen((char*)str, "r");
 
 	if (stream) {
 		while (!feof(stream))
 			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
 				pclose(stream);
 	}
	
}

string IMU::exec(string cmd) {
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	strcat((char*)cmd.c_str()," 2>&1");
	stream = popen((char*)cmd.c_str(), "r");

	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) !=NULL){
				data.append(buffer);
			}
			pclose(stream);
	}
	return data;
}

string IMU::getAccelX()
{
	string getAccelH="i2cget -y 2 0x68 0x3B";
	string getAccelL="i2cget -y 2 0x68 0x3C";
	return exec(getAccelH).substr(2,2)+exec(getAccelL).substr(2,2);
}
string IMU::getAccelY()
{
	string getAccelH="i2cget -y 2 0x68 0x3D";
	string getAccelL="i2cget -y 2 0x68 0x3E ";
	return exec(getAccelH).substr(2,2)+exec(getAccelL).substr(2,2);
}
string IMU::getAccelZ()
{
	string getAccelH="i2cget -y 2 0x68 0x3F";
	string getAccelL="i2cget -y 2 0x68 0x40";
	return exec(getAccelH).substr(2,2)+exec(getAccelL).substr(2,2);
}

string IMU::getGyroX()
{
	string getGyroH="i2cget -y 2 0x68 0x43";
	string getGyroL="i2cget -y 2 0x68 0x44";
	return exec(getGyroH).substr(2,2)+exec(getGyroL).substr(2,2);
}
string IMU::getGyroY()
{
	string getGyroH="i2cget -y 2 0x68 0x45";
	string getGyroL="i2cget -y 2 0x68 0x46";
	return exec(getGyroH).substr(2,2)+exec(getGyroL).substr(2,2);
}
string IMU::getGyroZ()
{
	string getGyroH="i2cget -y 2 0x68 0x47";
	string getGyroL="i2cget -y 2 0x68 0x48";
	return exec(getGyroH).substr(2,2)+exec(getGyroL).substr(2,2);
}
