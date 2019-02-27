#include "I2CSensor.h"
string I2CSensor::exec(char* cmd){
	//some processing variables
	string data = "";
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	// adds the final piece of the command for the read/write commands
	strcat(cmd, " 2>&1");
	//opens the results of the command after the command is run in linux 	
	stream = popen(cmd, "r");
	//if the stream properly opened and the end of the file hasn't been reached add the next character to a beffer string
	if(stream){
		while(!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);

		//after the file is read close the file
		pclose(stream);
	}
	// return the read string *note -UNPROCESSED still must be converted
	return data;
}

string I2CSensor::get(char* reg1, const char* reg2){
	//processing variables: str - command to be processed	    str2 - result of command
	char str[100];
	string str2 = "";

	//chain together the appropriate registers and commands to get the desired string command
	strcpy(str, cmdGet);
	strcat(str, reg1);
	strcat(str, reg2);

	// executes command and gets the result
	str2 = exec(str);

	//return result
	return str2;
} 
/*set - compiles setting commands
Inputs
reg1 - the address of the device on the i2c but
reg 2 - address of the information to be changed
Outputs
string - string of the command to be executed
*/

void I2CSensor::set(char* reg1, const char* reg2, int value){
	// processing variables str - string of command to run			 str2 - result of command
	char str[100];
	string str2;

	//chains together command and resister adresses to get the command string
	strcpy(str, cmdSet);
	strcat(str, reg1);
	strcat(str, reg2);
	strcat(str, to_string(value).c_str());


	//execute command
	exec(str);
}

