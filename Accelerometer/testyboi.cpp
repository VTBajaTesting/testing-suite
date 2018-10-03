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


Accel Aleft(al);
Accel Aright(ar);


while (count < 10)
{
Aleft.read();
cout << "LeftXYZ: " << stringToGs(Aleft.getX()) << " " << stringToGs(Aleft.getY()) << " " << stringToGs(Aleft.getZ()) << endl;
Aright.read();
cout << "RightXYZ: " << stringToGs(Aright.getX()) << " " << stringToGs(Aright.getY()) << " " << stringToGs(Aright.getZ()) << endl;
count++;
}

return 0;
}
