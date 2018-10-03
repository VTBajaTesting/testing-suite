
#include "accel.h"


/*Constructor- currently only works on -8 g to 8 g mode
Inputs
devADR - hex value of the accelerometers address (enter as 1C or 1D)
Outputs
-
*/
Accel::Accel(const char* devAdr)
{
strcpy(deviceADDR, " 0x");
strcat(deviceADDR, devAdr);
set(deviceADDR, PWR_Control, 1);
set(deviceADDR, Mode_Control, 2);

}

/*exec - runs the i2c commands that have been compiled as a string
Inputs
cmd - character array containing the commend to be executed
Outputs
string - the unproccessed string returned by the command 'should be in 0x-- form'
*/

string Accel::exec(char* cmd){
string data = "";
FILE * stream;
const int max_buffer = 256;
char buffer[max_buffer];
strcat(cmd, " 2>&1");
stream = popen(cmd, "r");

if(stream){
while(!feof(stream))
if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);

pclose(stream);
}
return data;
}




/*get - compiles reading commands
Inputs
reg1 - the address of the device on the i2c but
reg 2 - address of the desired information
Outputs
string - string of the command to be executed
*/


string Accel::get(char* reg1, const char* reg2){
char str[100];
string str2 = "";

strcpy(str, cmdGet);
strcat(str, reg1);
strcat(str, reg2);
str2 = exec(str);

return str2;
}





/*set - compiles setting commands
Inputs
reg1 - the address of the device on the i2c but
reg 2 - address of the information to be changed
Outputs
string - string of the command to be executed
*/

void Accel::set(char* reg1, const char* reg2, int value){
char str[100];
string str2;

strcpy(str, cmdSet);
strcat(str, reg1);
strcat(str, reg2);
strcat(str, to_string(value).c_str());

exec(str);

}





/*read - runs and sets all the Accel objects accelaration values to the sensor readings. Must be made called for new sensor readings.
*/


void Accel::read(){

accel_x = (get(deviceADDR, accel_xH)) + (get(deviceADDR, accel_xL));
accel_x = (get(deviceADDR, accel_xH)).substr(2, 2) + (get(deviceADDR, accel_xL)).substr(2, 1);
accel_y = (get(deviceADDR, accel_yH)).substr(2, 2) + (get(deviceADDR, accel_yL)).substr(2, 1);
accel_z = (get(deviceADDR, accel_zH)).substr(2, 2) + (get(deviceADDR, accel_zL)).substr(2, 1);
}

/*getX - accessor for the x acceleration component
Outputs
string - the signed hex number as a string
*/

string Accel::getX()
{
return accel_x;
}

/*getY - accessor for the y acceleration component
Outputs
string - the signed hex number as a string
*/


string Accel::getY()
{
return accel_y;
}

/*getZ - accessor for the z acceleration component
Outputs
string - the signed hex number as a string
*/


string Accel::getZ()
{
return accel_z;
}

/*stringToGs - nonmember function to convert the strings to signed ints
Inputs
str - the signed hex number as a string
Outputs
double - the reading of the acceleration in gs
*/
double stringToGs (string str)
{
  int num = stoi(str, nullptr, 16);
  double reading = (double)(num);
 if ( reading >= 2048)
  {
    reading = reading - 4096;
  }
  reading = reading  /  1024;
  return reading;
}
