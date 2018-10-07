#include "timer.h"
#include "../mutex.h"
#include "../Switch/switch.h"

extern std::mutex m;
Timer::Timer()
{
	start = chrono::steady_clock::now();
}

void Timer::readTimer()
{
	do
	{
//		m.unlock();
		readings.push_back(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count());
//		m.lock();
	}while(monitorSwitch() == 1);
//	m.unlock();
}

std::vector<int> Timer::getReadings()
{
	return readings;
}
