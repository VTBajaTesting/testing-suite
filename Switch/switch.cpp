int monitorSwitch()
{
	stringstream fileName;
	fileName<<"/sys/class/gpio/gpio48/value";
	FILE *file=fopen(fileName.str().c_str(),"r");
        if(!file)
        {
        	perror("File Switch ");
                return -1;
        }
        rewind(file);

	//read 1 value from teh gpio pin
        char* buffer;
	buffer=(char*)malloc(sizeof(char));
        size_t reading=fread(buffer,1,1,file);
	//close the file
        fclose(file);
	//std::cout<<"Done monitoring"<<std::endl;
	return atoi(buffer);
}
