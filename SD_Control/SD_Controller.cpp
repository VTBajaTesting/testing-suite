#include "SD_Controller.h"


using namespace std;


SD_Controller::SD_Controller(string _filename, int _updateNum, long _systemUpTime, int _numOfLinPot,
	int _numOfAccel, int _numOfIMU, int _otherShit)
{
	my_filename     = _filename;
	my_updateNum    = _updateNum;
	my_systemUpTime = _systemUpTime;
	my_numOfLinPot  = _numOfLinPot;
	my_numOfAccel   = _numOfAccel;
	my_numOfIMU     = _numOfIMU;
	my_otherShit    = _otherShit;


	extended_Filename = my_filename + to_string(my_updateNum); //this includes the filename and any revision number
	                                                           //this ensures that we do not overwrite any prior 
	                                                           //logs or tests


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
}
bool SD_Controller::close_File()
{
	//not valid
	myFile.close();
}
bool SD_Controller::write_Data(string _information)
{

}