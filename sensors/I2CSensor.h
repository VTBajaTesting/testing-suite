#ifndef I2CSENSOR
#define I2CSENSOR
class I2CSensor{//not instantiable
	private:
		const char* cmdGet = "i2cget -y 2";
		const char* cmdSet = "i2cset -y 2";
	public:
		string exec(char* cmd);
		string get(char* reg1,const char* reg2);
		void set(char*,const char*,int);
		virtual string* read()=0;
};
#endif
