#include<chrono>
#include<vector>

#ifndef TIMER
#define TIMER

class Timer
{
	private:
		std::vector<int> readings;
		std::chrono::time_point<std::chrono::steady_clock> start;
	public:
		Timer();
		std::vector<int> getReadings();
		void readTimer();
};
#endif
