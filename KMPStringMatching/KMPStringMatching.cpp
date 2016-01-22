// KMPSearch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<iostream>
#include<stdlib.h>
#include "KMPSearch.h"
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
    SequentialKMP obj;
	obj.setTextViaFile("C:\\Users\\hp\\Desktop\\Prateek\\multitext.txt");
	obj.setPatternViaFile("C:\\Users\\hp\\Desktop\\Prateek\\pttrn.txt");
	// for sequential
	//obj.SearchString();
	//cout<<obj.getNumberOfOccurences();
	//cout<<"\n"<<obj.getExecutionTimeInSeconds();
	//cout<<"\n"<<obj.getExecutionTimeInMiliSeconds();
	//cout<<"\n"<<obj.getExecutionTimeInNanoSeconds();
   
	//obj.searchWithNumberOfThreads(20);
	obj.searchWithNumberOfOpenCLWorkItem();
	cout<<obj.getNumberOfOccurences();
	//cout<<"\n"<<obj.getExecutionTimeInSeconds();
	//cout<<"\n"<<obj.getExecutionTimeInMiliSeconds();
	//cout<<"\n"<<obj.getExecutionTimeInNanoSeconds();
	getchar();
	return 0;
}
