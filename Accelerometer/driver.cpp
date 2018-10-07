#include "accel.cpp"
#include "accel.h"


using namespace std;

int main() {
	int count = 0;
	while (1 == 1) {
		if (count % 25 == 0) {
			Accel test(2, 0x1d);
			test.setRange(1);
			std::cout << "X: " << test.getX() << std::endl;
			std::cout << "Y: " << test.getY() << std::endl;
			std::cout << "Z: " << test.getZ() << std::endl;
		}
		count++;
	}
	return 0;
}
