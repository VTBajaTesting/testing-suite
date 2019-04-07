#include "SD_Controller.h"
#include <sstream>
#include<chrono>
using namespace std;

SD_Controller::SD_Controller(string _filename, long _systemUpTime,std::vector<Sensor*>s,std::vector<I2CSensor*> i)
{
	cout<<"SD Controller Working"<<endl;
	my_filename     = _filename;
	system_up_time  = _systemUpTime;
	for(int j=0;j<s.size();j++){
		sensors.push_back(s.at(j));
	}
	for(int k=0;k<i.size();k++){
		i2csensors.push_back(i.at(k));
	}
	
	gps=new GPS();
	extended_Filename = my_filename+std::to_string(_systemUpTime)+".csv"; //this includes the filename and any revision number
	                                                           //this ensures that we do not overwrite any prior 
	                                                           //logs or tests

	string gps_name = my_filename+std::to_string(_systemUpTime)+".csv";
	if (myFile.is_open()) //check if file is open for whatever reason beforehand
	{
		myFile.close();
	}
	myFile.open(extended_Filename); //open new file with _filename and _updateNum
	gpsFile.open(gps_name);
	if (myFile.is_open()) //make sure the file is actually open
	{
		myFile.close();
		myFile.open(extended_Filename);
	}
	cout<<"SD Controller Done"<<endl;
}

bool SD_Controller::open_File()
{
	//not valid
	myFile.open(extended_Filename);
//TODO:Add return statement
}
bool SD_Controller::close_File()
{
	//not valid
	myFile.close();
}
bool SD_Controller::write_Data(int time)
{
	stringstream toWrite;
	//std::thread gps=std::thread(&SD_Controller::write_GPS,this);
	//write_GPS();
	toWrite<<time<<",";
	for(int i=0;i<sensors.size();i++){
		int* read=sensors.at(i)->read();
		for(int j=0;j<sizeof(read)/sizeof(int);j++){
			toWrite << *(read+j) << ",";
		}
	}
	for(int i=0;i<i2csensors.size();i++){
		string* read= i2csensors.at(i)->read();
		for(int j=0;j<sizeof(read)/sizeof(std::string);j++){
			toWrite << *(read+j) << ",";
		}
	}
	if(time%5000==0){
		double* gpsReadings = gps->parse();
		toWrite<<*(gpsReadings)<<","<<*(gpsReadings+1)<<","<<*(gpsReadings+2);
	}
	myFile<<"\n"<<toWrite.rdbuf();
	/*accelerometerR.read();
	accelerometerL.read();
	//string* imuReadings=imu->read();
	short potent[4]={fRPotentiometer.getVal(),fLPotentiometer.getVal(),bRPotentiometer.getVal(),bLPotentiometer.getVal()};
	//store each line in a string stream to be written to the ofstream
	//I think that this is easier to do than passing in information as 
	//a parameter. I guaranteed that the sensors are reasonably synced by
	//making each sensor have the same number of calls.
	toWrite<<time<<","<<potent[0]<<","<<potent[1]<<","<<potent[2]<<","<<potent[3]<<",";
	toWrite<<accelerometerR.getX()<<","<<accelerometerR.getY()<<","<<accelerometerR.getZ()<<",";
	toWrite<<accelerometerL.getX()<<","<<accelerometerL.getY()<<","<<accelerometerL.getZ();
	//toWrite<<","<<*(imuReadings)<<","<<*(imuReadings+1)<<","<<*(imuReadings+2)<<","<<*(imuReadings+3)<<","<<*(imuReadings+4)<<","<<*(imuReadings+5)<<"\n";
	//std::cout<<"imu"<<imu.getAccelX()<<std::endl;
	myFile<<toWrite.rdbuf();*/
	//gps.join();
}

void SD_Controller::write_GPS(){
	stringstream toWrite;
        double* gpsReadings = gps->parse();
        toWrite<<*(gpsReadings)<<","<<*(gpsReadings+1)<<","<<*(gpsReadings+2);
        gpsFile<<"\n"<<toWrite.rdbuf();
}
