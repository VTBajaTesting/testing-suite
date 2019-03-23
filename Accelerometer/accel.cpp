//Accelorometer object code 
//Written by: Matthew Herrity
//Last Editted: 10/5/2018
#include "accel.h"
#include "../sensors/I2CSensor.cpp"

/*Constructor- currently only works on -8 g to 8 g mode
Inputs
devADR - hex value of the accelerometers address (enter as 1C or 1D)
Outputs
-
*/
Accel::Accel(const char* devAdr="1c", int r = 8)
{
//sets the device adress string to ' 0x1c' or ' 0x1d'
strcpy(deviceADDR, " 0x");
strcat(deviceADDR, devAdr);
//sets the range equal to the parameter
range = r;
//uses if statements to setup the register equivalent to the inputed range
if(range == 4)
{
r = 1;
}
else if(range == 2)
{
r = 0;
}
else
{
range = 8;
r = 2;
}

//writes the scaleing register to have the proper inputed scale
set(deviceADDR, Mode_Control, r);
//writes the sleep register to be in awake mode
set(deviceADDR, PWR_Control, 1);

}

/*Default constructor*/
Accel::Accel(){}

/*
~Accel - frees all allocated memory
*/

Accel::~Accel()
{
set(deviceADDR, PWR_Control, 0);
set(deviceADDR, Mode_Control, 0);
delete [] deviceADDR;
}


/*read - runs and sets all the Accel objects accelaration values to the sensor readings. Must be made called for new sensor readings.*/


string* Accel::read(){


//for each command read the registers containing the readings and smash togeter the necessary information
accel_x = (get(deviceADDR, accel_xH)).substr(2, 2) + (get(deviceADDR, accel_xL)).substr(2, 1);
accel_y = (get(deviceADDR, accel_yH)).substr(2, 2) + (get(deviceADDR, accel_yL)).substr(2, 1);
accel_z = (get(deviceADDR, accel_zH)).substr(2, 2) + (get(deviceADDR, accel_zL)).substr(2, 1);
static string toReturn[3]={accel_x,accel_y,accel_z};
return toReturn;
}

/***********************************************
unused methods, but they may be useful later.
************************************************/
/*getX - accessor for the x acceleration component
Outputs
string - the signed hex number as a string
*/

string Accel::getX()
{
//return the x reading
return accel_x;
}

/*getY - accessor for the y acceleration component
Outputs
string - the signed hex number as a string
*/


string Accel::getY()
{
//return the y reading
return accel_y;
}

/*getZ - accessor for the z acceleration component
Outputs
string - the signed hex number as a string
*/


string Accel::getZ()
{
//return the z reading
return accel_z;
}

int Accel::getRange()
{
//return the range value
return range;
}

/*stringToGs - nonmember function to convert the strings to signed ints
Inputs
str - the signed hex number as a string
Outputs
double - the reading of the acceleration in gs
*/
double stringToGs (string str, int range)
{
	// convert the string of hex to an unsigned int
  int num = stoi(str, nullptr, 16);
//convert the in to a double
  double reading = (double)(num);
// if the most significant bit is one (>2048) the number is negative so we take the twos complement
 if ( reading >= 2048)
  {
    reading = reading - 4096;
  }
// this divides by a conversion factor based on the range
  reading = reading /(2048  / range) ;
  return reading;
}
