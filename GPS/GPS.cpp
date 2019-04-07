#include "GPS.h"
char* GPS::read(){
	std::cout<<"------BEEP BAP BOOP PYTHON INITIALIZING----------"<<std::endl;
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\"./GPS/\")");
	PyObject* moduleString = PyString_FromString((char*)"readGPSOnce");
	PyObject* module = PyImport_Import(moduleString);
	if(module == NULL){
		PyErr_Print();
		//return -1;
	}
	PyObject* pFunc = PyObject_GetAttrString(module,"read_once");
	if(pFunc == NULL){
		std::cerr<<"No function called \"read_once\" in readGPSOnce"<<std::endl;
		//return -1;
	}
	PyObject* result = PyObject_CallObject(pFunc,NULL);
	if(result == NULL){
		std::cerr<<"No result returned from GPS. Check your connection and hardware."<<std::endl;
		//return -1;
	}
	char* s = PyString_AsString(result);
	Py_Finalize();
	return s;
}

double* GPS::parse(){
	static double toReturn[3];
	char* result=read();
	std::string res(result);
	int column=0;
	//std::stringstream toReturn;
	if(res.find("RMC") != std::string::npos && res.find("A") != std::string::npos){
		for(int i = 0; i < res.size(); i++){
			char c=res.at(i);
			if(c == ','){
				column++;
			}
			//parse latitude in column 3
			else if(column==3){
				std::string s;
				int n=i;
				for(i;i<n+2;i++){
					s+=res.at(i);
				}
				int deg=atoi(s.c_str());
				s.clear();
				n=i;
				for(i;i<n+7;i++){
					s+=res.at(i);
				}
				float min=atof(s.c_str());
				s.clear();
				double decimal_degrees = deg+min/60;
				toReturn[0]=decimal_degrees;
			}
			//prase the longitude in column 5
			else if(column==5){
				std::string s;
				int n=i;
                                for(i;i<n+3;i++){
                                        s+=res.at(i);
                                }
                                int deg=atoi(s.c_str());
                                s.clear();
                                n=i;
                                for(i;i<n+7;i++){
                                        s+=res.at(i);
                                }
                                float min=atof(s.c_str());
                                s.clear();
                                double decimal_degrees = deg+min/60;
				toReturn[1]=decimal_degrees;
			}
			else if(column==7){
				std::string s;
				int endloc=res.find(",",i);
				for(i;i<endloc;i++){
					s+=res.at(i);
				}
				double speed = atof(s.c_str())*1.852;//speed units is km/h
				toReturn[2]=speed;
			}
		}
		return toReturn;
	}
	static double toRet[3] = {-1.0,-1.0,-1.0};
	return toRet;

}
