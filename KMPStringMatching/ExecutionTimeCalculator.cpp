#include "StdAfx.h"
#include "ExecutionTimeCalculator.h"

ExecutionTimeCalculator::ExecutionTimeCalculator(void)
{
}

ExecutionTimeCalculator::~ExecutionTimeCalculator(void)
{
}
	
void ExecutionTimeCalculator::start()
{
	startClock = clock();
}
void ExecutionTimeCalculator::stop()
{
	finishClock = clock();
}

double ExecutionTimeCalculator::executionTimeInSeconds()
{
	double timeValue;
    timeValue = finishClock - startClock ;
    timeValue= (timeValue/CLOCKS_PER_SEC);
	return timeValue;
}
double ExecutionTimeCalculator::executionTimeInMiliSeconds()
{
	double timeValue;
    timeValue = finishClock - startClock ;
    timeValue= (timeValue/CLOCKS_PER_SEC)*1000;
	return timeValue;
}
double ExecutionTimeCalculator::executionTimeInMicroSeconds()
{
	double timeValue;
    timeValue = finishClock - startClock ;
    timeValue= (timeValue/CLOCKS_PER_SEC)*1000000;
	return timeValue;
}
double ExecutionTimeCalculator::executionTimeInNanoSeconds()
{
	double timeValue;
    timeValue = finishClock - startClock ;
    timeValue= (timeValue/CLOCKS_PER_SEC)*1000000000;
	return timeValue;
}	
double ExecutionTimeCalculator::executionTimeInClocks()
{
	double timeValue;
	timeValue = finishClock - startClock ;
	return timeValue;
}