#include "LP.h"




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
int* LinPot::read(){

	//get location of ADC Port file
	stringstream fileName;
	fileName<<"/sys/bus/iio/devices/iio:device0/in_voltage"<<adcPort<<"_raw";
	FILE *file=fopen(fileName.str().c_str(),"r");
        if(!file)
        {
        	perror("Linear Potentiometer ");
		static int readings[1]={-1};
                return readings;
        }
        rewind(file);

	//read 1 value from teh adc pin
        char* buffer;
	buffer=(char*)malloc(sizeof(char)*4);
        size_t reading=fread(buffer,1,4,file);
	
	//close the file
        fclose(file);
	static int readings[1]={atoi(buffer)};
	return readings;
}


