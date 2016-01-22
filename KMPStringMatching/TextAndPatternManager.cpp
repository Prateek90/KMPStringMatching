#include "StdAfx.h"
#include "TextAndPatternManager.h"
// constructors and destructors
TextAndPatternManager::TextAndPatternManager(char * txtFilename,char *paternFileName,PatternMode mode)
{
	textFileName = new char[strlen(txtFilename)+1];
	patternFileName = new char[strlen(paternFileName)+1];
	strcpy(textFileName,txtFilename);
	strcpy(patternFileName,paternFileName);
	patternMode = mode;
}

TextAndPatternManager::~TextAndPatternManager(void)
{
	delete textFileName;
    delete patternFileName;
	delete textData;
	delete patternData;
}
// Load Text Data
void TextAndPatternManager::loadTextData()
{
   fstream fp;
   fp.open(textFileName,ios::in|ios::out);
   if(fp){
	  fp.seekg(0,ios::end);     
	  long m=fp.tellg();
      textData = new char[m+1];
	  textData[m]='\0';
	  fp.close();
      fp.open(textFileName,ios::in|ios::out);
      fp.read(textData,m);
	  fp.close();
   }
   else{
     cout<<"text file not open";
   }
}
// Getter function for text
char* TextAndPatternManager::getTextData()
{
  return textData;
}
// Load pattern Data
void TextAndPatternManager::loadPatternData()
{
   fstream fp;
   fp.open(patternFileName,ios::in|ios::out);
   if(fp){
	  fp.seekg(0,ios::end);     
	  long m=fp.tellg();
      patternData = new char[m+1];
	  patternData[m]='\0';
	  fp.close();
      fp.open(patternFileName,ios::in|ios::out);
      fp.read(patternData,m);
	  fp.close();
      if(patternMode==MULTIPLE)
      {
	    // fucntion to file the data in patternArray;
        fillPatternArray();
      }
	  else
	  {
	  //do nothing
	  }
   }
   else{
     cout<<"pattern file not open";
   }
  
}
// Getter function for pattern
char* TextAndPatternManager::getPatternData()
{
   return patternData;
}
void TextAndPatternManager::fillPatternArray()
{
  int PatternDataSize = strlen(patternData);
  int start = 0;
  int end =0;
  int j=0;
  int k=0;
  for( int i=0;i<PatternDataSize;i++)
  {
	  if(patternData[i]!='$'){
		 patternArray[j][k]=patternData[i];
		 k++;
	  }
	  else{
		  patternArray[j][k]='\0';
		  k=0;
		  j++;
	  // do nothing
	  }
  }
  patternArray[j][k]='\0';
  numOfPattern = j+1;
}
int TextAndPatternManager::getNumberOfPattern(){
 return numOfPattern;
}
