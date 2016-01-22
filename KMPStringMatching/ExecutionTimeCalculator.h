#pragma once
#include<time.h>
#include<ctime>
class ExecutionTimeCalculator
{
clock_t startClock;
clock_t finishClock;
double timeCount;

public:
	ExecutionTimeCalculator(void);
	~ExecutionTimeCalculator(void);
	void start();
	void stop();
	double executionTimeInSeconds();
	double executionTimeInMiliSeconds();
	double executionTimeInMicroSeconds();
	double executionTimeInNanoSeconds();
	double executionTimeInClocks();
};
