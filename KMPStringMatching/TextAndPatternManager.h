#pragma once
#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;
#define PATTERN_NUMBER 100

enum PatternMode{
	SINGLE,
	MULTIPLE
}; 
class TextAndPatternManager
{
	// For input files names ( text and pattern both)
	char *textFileName;
	char *patternFileName;

	// Pattern mode: if pattern is MULTIPLE than in pattern file patterns are seperated by "\n"
    PatternMode patternMode;
    
    // For input files Data ( text and pattern both)
	char *textData;
	char *patternData;
	//Multi pattern Array
	char patternArray[PATTERN_NUMBER][100];
	int numOfPattern;
	void fillPatternArray();

public:
	// constructors and destructors
	TextAndPatternManager(char * txtFilename,char *paternFileName,PatternMode mode);
	~TextAndPatternManager(void);

	// Load Text Data
    void loadTextData();
	// Load pattern Data
	void loadPatternData();
	// Getter function for text
	char* getTextData();
	// Getter function for text
	char* getPatternData();
	// Set pattern Data is multipattern Data
	int getNumberOfPattern();

 };
