#ifndef ACCEL
#define ACCEL

class Accel {

private:
	int I2CBus, I2CAddress;
	char dataBuffer[0x7f];

	int accelerationX;//TODO:gotta initialize these, for now we can say they are undefined until they are read in, which actually kinda makes sense
	int accelerationY;
	int accelerationZ;

	int file;
	int range;

	int  convertAcceleration(int, int);
	int  writeByte(char address, char value);
public:
	Accel(int bus, int address);
	void readData();

	void setRange(int range);
	int getRange();
	//TODO:put these in accel.cpp
	int getX() { return accelerationX; }
	int getY() { return accelerationY; }
	int getZ() { return accelerationZ; }
};

#endif
