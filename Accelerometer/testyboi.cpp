#include "accel.cpp"

using namespace std;

int count;
const char* ar;
const char* al;

int main()
{
count = 0;
al = "1d";
ar = "1c";


Accel Aleft(al, 8);
Accel Aright(ar, 8);


while (count < 10)
{
Aleft.read();
cout << "LeftXYZ: " << stringToGs(Aleft.getX(), Aleft) << " " << stringToGs(Aleft.getY(), Aleft) << " " << stringToGs(Aleft.getZ(), Aleft) << endl;
Aright.read();
cout << "RightXYZ: " << stringToGs(Aright.getX(), Aright) << " " << stringToGs(Aright.getY(), Aright) << " " << stringToGs(Aright.getZ(), Aright) << endl;
count++;
}

return 0;
}
