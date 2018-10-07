#include "LP.h"
#include <sstream>
#include "../mutex.h"

extern std::mutex m;
LinPot::LinPot()
{
	val=-1;
	adcPort=0;
}
LinPot::LinPot(short value, short adcPort){
    	val=value;
        this->adcPort=adcPort;
}

//returns the value of the linear potentiometer
void LinPot::readLP(){

	//get location of ADC Port file
	do
	{
	//	m.unlock();
		std::stringstream fileName;
		fileName<<"/sys/bus/iio/devices/iio:device0/in_voltage"<<adcPort<<"_raw";
		FILE *file=fopen(fileName.str().c_str(),"r");
       	 	if(!file)
       	 	{
       	 		perror("Linear Potentiometer ");
       	        	return;
       		}
        	rewind(file);

		//read 1 value from teh adc pin
        	char* buffer;
		buffer=(char*)malloc(sizeof(char)*4);
        	size_t reading=fread(buffer,1,4,file);
	
		//close the file
	      	fclose(file);
		readings.push_back(atoi(buffer));
		//m.lock();
	}while(monitorSwitch() == 1);
	//m.unlock();
}

std::vector<int> LinPot::getReadings()
{
	return readings;
}

