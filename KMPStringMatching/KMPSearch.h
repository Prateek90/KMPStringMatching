#pragma once
#include "ExecutionTimeCalculator.h"
#include "TextAndPatternManager.h"
#include "CL\cl.h"
#include <stdio.h>
#include <windows.h>          // for HANDLE
#include <process.h>          // for _beginthread()
#include<math.h>
#define MAX_THREAD_COUNT 100
struct threadData{
 char* text;
 char* pattern;
 long *numberofCount;
 long startPos;
 long endPos;
 int *PrefixArray;
 //add Prefix Array
};
static CRITICAL_SECTION m_CriticalSection;
class SequentialKMP
{
	char *textFileName;
	char *patternFileName;
	long numberOfOccurence;
	ExecutionTimeCalculator timeObj;
	void search(char* text, char* pattern);
    // prefix function
	int * PrefixArray;
	void calculatePrefixFunction(char* pattern);
	void KMPSearch(char * text, char* pattern);

	HANDLE threadArray[MAX_THREAD_COUNT];
public:
	SequentialKMP(void);
	~SequentialKMP(void);

	void setTextViaFile(char* fileName);
	void setPatternViaFile(char* fileName);
	void SearchString();
	long getNumberOfOccurences();
	char * getOccurrecePosition();
	double getExecutionTimeInSeconds();
	double getExecutionTimeInMiliSeconds();
	double getExecutionTimeInMicroSeconds();
	double getExecutionTimeInNanoSeconds();

	static unsigned __stdcall ThreadSearch(void * threadObj);
    void searchWithNumberOfThreads(long numberOfThreads);

	// for Open CL
	//void searchWithNumberOfOpenCLWorkItem(long numberOfWorkItem);
	void searchWithNumberOfOpenCLWorkItem();
	long calculateWorkItems(long textLength,long patternLength);
};


	
	
    