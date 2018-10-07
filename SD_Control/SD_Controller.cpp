#include "SD_Controller.h"
#include <sstream>

using namespace std;


SD_Controller::SD_Controller(string _filename, short _updateNum, long _systemUpTime, short _numOfLinPot,
	short _numOfAccel, short _numOfIMU, short _otherShit)
{
	my_filename     = _filename;
	my_updateNum    = _updateNum;
	my_systemUpTime = _systemUpTime;
	my_numOfLinPot  = _numOfLinPot;
	my_numOfAccel   = _numOfAccel;
	my_numOfIMU     = _numOfIMU;
	my_otherShit    = _otherShit;

	//need to make claibration curves for linear potentiometers
	fRPotentiometer=LinPot(-1,1);
	fLPotentiometer=LinPot(-1,0);
	bRPotentiometer=LinPot(-1,3);
	bLPotentiometer=LinPot(-1,2);
	//imu=IMU();
	extended_Filename = my_filename + to_string(my_updateNum)+".csv"; //this includes the filename and any revision number
	//this ensures that we do not overwrite any prior 
	//logs or tests
	//accelerometerR.setRange(1);
	//accelerometerL.setRange(1);

	if (myFile.is_open()) //check if file is open for whatever reason beforehand
	{
		myFile.close();
	}
	myFile.open(extended_Filename); //open new file with _filename and _updateNum

	//depending on the parameters/arguments provided, create comma-deliminated value headers in .txt file
	string tempStorage = "";
	if (myFile.is_open()) //make sure the file is actually open
	{
		////Scratch this////myFile << "Empty Box,"; //The first "box" in an excel sheet is usually empty. This is comma-deliminated
		myFile << "Time";

		if (my_numOfLinPot >= 1)
		{
			for (int i = 0; i < my_numOfLinPot; i++)
				tempStorage = tempStorage + "LinearPot" + to_string(1 + i) + ",";
			myFile << tempStorage;
		}
		//myFile << tempStorage; 
		tempStorage = ""; //Force of habit - always reset a reused variable whether it changes or not
		
		/// Accelerometer requires X,Y,Z headers
		if (my_numOfAccel >= 1)
		{
			for (int i = 0; i < my_numOfAccel; i++)
				tempStorage = (tempStorage + "Accelerometer" + to_string(1 + i) + "_x,Acc_y,Acc_z,");
			myFile << tempStorage;
		}
		tempStorage = "";

		/// IMU Requires multiple columns
		/// x, y, z, xV, yV, zV
		if (my_numOfIMU >= 1)
		{
			for (int i = 0; i < my_numOfIMU; i++)
				tempStorage = tempStorage + "IMU" + to_string(1 + i) + ",";
			myFile << tempStorage;
		}
		tempStorage = "";

		if (my_otherShit >= 1)
		{
			for (int i = 0; i < my_otherShit; i++)
				tempStorage = tempStorage + "OtherShit" + to_string(1 + i) + ",'";
			myFile << tempStorage;
		}
		tempStorage = "";

		myFile.close();
		myFile.open(extended_Filename);
	}
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
	imu->readImu();
	//accelerometerL.readData();
	//accelerometerR.readData();
	short potent[4]={fRPotentiometer.getVal(),fLPotentiometer.getVal(),bRPotentiometer.getVal(),bLPotentiometer.getVal()};
	//store each line in a string stream to be written to the ofstream
	//I think that this is easier to do than passing in information as 
	//a parameter. I guaranteed that the sensors are reasonably synced by
	//making each sensor have the same number of calls.
	toWrite<<time<<","<<potent[0]<<","<<potent[1]<<","<<potent[2]<<","<<potent[3]<<","<<imu->getGyroX()<<","<<imu->getGyroY()<<","<<imu->getGyroZ()<<",";
	toWrite<<imu->getAccelX()<<","<<imu->getAccelY()<<","<<imu->getAccelZ()<<","<<imu->getTemp()<<"\n";
	//toWrite<<accelerometerL.getX()<<","<<accelerometerL.getY()<<","<<accelerometerL.getZ()<<",";
	//toWrite<<accelerometerR.getX()<<","<<accelerometerR.getY()<<","<<accelerometerR.getZ()<<"\n";
	myFile<<toWrite.rdbuf();
}
