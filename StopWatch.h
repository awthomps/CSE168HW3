#ifndef CSE168_STOPWATCH_H
#define CSE168_STOPWATCH_H

#include <time.h>
#include <string>
#include <iostream>


class StopWatch
{
public:
	StopWatch() {};
	~StopWatch() {};

	void StartTime(std::string message) {
		startTime = time(NULL);
		std::cout << message << std::endl;
	}
	void CheckTime(std::string message) {
		std::cout << message << std::endl << "Time elapsed: " << difftime(time(NULL), startTime) << " seconds." << std::endl;
	}


private:
	time_t startTime;
};

#endif

