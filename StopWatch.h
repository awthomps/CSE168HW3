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
		int totalTime = difftime(time(NULL), startTime);
		std::cout << message << std::endl;
		if (totalTime < 60)  std::cout << "Time elapsed: " << totalTime << " seconds." << std::endl;
		else std::cout << "Time elapsed: " << totalTime/60 << " minutes and "  << totalTime%60 << " seconds." << std::endl;
	}


private:
	time_t startTime;
};

#endif

