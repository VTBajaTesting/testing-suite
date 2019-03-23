#include <string>

#ifndef I2CSENSOR
#define I2CSENSOR
using namespace std;
class I2CSensor{//don't instantiate
	private:
		const char* cmdGet = "i2cget -y 2";
		const char* cmdSet = "i2cset -y 2";
	public:
		I2CSensor(){};
		string exec(char* cmd);
		string get(char* reg1,const char* reg2);
		void set(char*,const char*,int);
		string* read(){};
};
#endif
