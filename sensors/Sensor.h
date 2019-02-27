#ifndef SENSOR
#define SENSOR
class Sensor{//not instantiable
	public:
		virtual int* read()=0;
};
#endif
