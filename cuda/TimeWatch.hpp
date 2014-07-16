#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <ctime>

typedef unsigned long long uint64;

class TimeWatch
{
public:
	TimeWatch(){};
	~TimeWatch(){};

	void startCount(){
		gettimeofday(&tv, NULL);
		lastTimeStamp = tv.tv_sec * (uint64) 1000000 + tv.tv_usec;
	}
	double getElapsedTime(){
		gettimeofday(&tv, NULL);
		uint64 newTimeStamp = tv.tv_sec * (uint64) 1000000 + tv.tv_usec;
		double elapsed_time = (double)1.0 / 1000.0 * (newTimeStamp - lastTimeStamp);
		newTimeStamp = lastTimeStamp;
		return elapsed_time;
	}
private:
	struct timeval tv;
	uint64 lastTimeStamp;
};
