#include "StdAfx.h"
#include "KMPSearch.h"

SequentialKMP::SequentialKMP(void)
{
	numberOfOccurence =0;

    InitializeCriticalSection(&m_CriticalSection);
}

SequentialKMP::~SequentialKMP(void)
{
	delete textFileName;
	delete patternFileName;
	free(PrefixArray);

	// Release resources used by the critical section object.
    DeleteCriticalSection(&m_CriticalSection);
}

void SequentialKMP::setTextViaFile(char* fileName){
    textFileName = new char[strlen(fileName)+1];
	strcpy(textFileName,fileName);
	
}
void SequentialKMP::setPatternViaFile(char* fileName){
	patternFileName = new char[strlen(fileName)+1];
	strcpy(patternFileName,fileName);
}
// prefix function
void SequentialKMP::calculatePrefixFunction(char* pattern)
{
  // calculation for the prefix functions
	int patternLength = strlen(pattern);
	PrefixArray = (int*) malloc((patternLength+1) * sizeof(int));
	PrefixArray[0]=-1;
	for(int i=0;pattern[i]!='\0';i++){
	   PrefixArray[i+1]=PrefixArray[i]+1;
	   while((PrefixArray[i+1]>0)&& (pattern[i]!=pattern[PrefixArray[i+1]-1]))
             PrefixArray[i+1]=PrefixArray[PrefixArray[i+1]-1]+1;
	}
}
void SequentialKMP::KMPSearch(char * text, char* pattern)
{
  //searching
	numberOfOccurence=0;
	long i=0,j=0;
	long textLength=strlen(text);
	long patLength = strlen(pattern);
	for(i=j=0;i<textLength;)
	{
	  if((j<0)||(text[i]==pattern[j]))
	  {
		j++;
		i++;
		if(j==patLength)
		{
		  numberOfOccurence++;
		  j=0;
		 
		}
	  }
	  else
	  {
		  j=PrefixArray[j];
	  }
    }
}
void SequentialKMP::search(char* text, char* pattern)
{  
   // prefix fucntion calculate
    calculatePrefixFunction(pattern);
	//start searching 
	timeObj.start();
    // searching logic fucntion
	KMPSearch(text,pattern);
	timeObj.stop();
	//end searching
}
void SequentialKMP::SearchString(){

	
	TextAndPatternManager textPatternObj(textFileName,patternFileName,SINGLE);
	textPatternObj.loadTextData();
	char* text = textPatternObj.getTextData();
	textPatternObj.loadPatternData();
	char* pattern = textPatternObj.getPatternData();
	search(text,pattern);
	
}

long SequentialKMP::getNumberOfOccurences(){
	return numberOfOccurence;
}
char * SequentialKMP::getOccurrecePosition(){

   return NULL;
}
double SequentialKMP::getExecutionTimeInSeconds(){
	return (timeObj.executionTimeInSeconds());
}
double SequentialKMP::getExecutionTimeInMiliSeconds(){
	return (timeObj.executionTimeInMiliSeconds());
}
double SequentialKMP::getExecutionTimeInMicroSeconds(){
	return (timeObj.executionTimeInMicroSeconds());
}
double SequentialKMP::getExecutionTimeInNanoSeconds(){
	return (timeObj.executionTimeInNanoSeconds());
}
unsigned __stdcall  SequentialKMP::ThreadSearch(void * threadObj)
{
   threadData *obj =(threadData*)threadObj;
   //cout<<"threaad enter";
   //long length =obj->endPos;
   long patLength = strlen(obj->pattern);
   int i;
   i = obj->startPos;
   long  textLength =obj->endPos;
   long j=0;
	for(j=0;i<textLength;)
	{
	  if((j<0)||(obj->text[i]==obj->pattern[j]))
	  {
		j++;
		i++;
		if(j==patLength)
		{
		  //EnterCriticalSection( &m_CriticalSection ); 
		  (*(obj->numberofCount))++;
		  //LeaveCriticalSection( &m_CriticalSection );
		  j=0;
		 
		}
	  }
	  else
	  {
		  j=obj->PrefixArray[j];
	  }
    }
  
	//cout<<"\nthread time"<<time.executionTimeInSeconds()<<"\n";
    delete obj;
	return 1;          // the thread exit code
}
void SequentialKMP::searchWithNumberOfThreads(long numberOfThreads)
{
    TextAndPatternManager textPatternObj(textFileName,patternFileName,SINGLE);
	textPatternObj.loadTextData();
	char* text = textPatternObj.getTextData();
	textPatternObj.loadPatternData();
	char* pattern = textPatternObj.getPatternData();
    numberOfOccurence =0;
	long textLength = strlen(text);
    long divisionFactor =textLength/numberOfThreads;
	long patternLength=strlen(pattern);
	calculatePrefixFunction(pattern);
    
	for(long i =0; i<numberOfThreads;i++)
	{
	threadData *obj ;
	obj = new threadData();
	if(i!=numberOfThreads-1)
	    obj->endPos = (i+1)*(divisionFactor)+patternLength;
	else
		obj->endPos = textLength;
	obj->startPos =i*(divisionFactor) + 1;
	obj->text = text;
	obj->pattern = pattern;
	obj->numberofCount =&numberOfOccurence;
	obj->PrefixArray=&PrefixArray[0];

	HANDLE   hth;
    unsigned  uiThread1ID;
    
    hth = (HANDLE)_beginthreadex( NULL,         // security
                                   0,            // stack size
								   SequentialKMP::ThreadSearch,
                                   (void*)obj,           // arg list
                                   CREATE_SUSPENDED,  // so we can later call ResumeThread()
                                   &uiThread1ID );
   
    if ( hth == 0 )
        printf("Failed to create thread 1\n");
    
	 threadArray[i]= hth;
	 

	}
	DWORD   dwExitCode;

    timeObj.start();
    for(long i =0; i<numberOfThreads;i++)
	{
	  ResumeThread( threadArray[i] );
	}
	WaitForMultipleObjects(numberOfThreads,&threadArray[0],TRUE,INFINITE);
	for(long i =0; i<numberOfThreads;i++)
	{
	  GetExitCodeThread( threadArray[i], &dwExitCode );
	  CloseHandle(threadArray[i]);
	}
	timeObj.stop();
}
const char *source =
"__kernel void KMPSearch(__global const char *text, \n"
"__global const char *pattern, \n"
"__global long *patCount, \n"
"__global long *impData,\n"
"__global int* Prefix)	\n"
"{ \n"
" int gid = get_global_id(0); \n"
" impData;\n"//0:textLength,1:patternLength,2:divisionFactor,3:numberOfthreads
" long start=0,j=0;\n"
" long textLength1=impData[0];\n"
" long patternLength=impData[1];\n"
" long divisionFactor=impData[2];\n"
" long numberOfThreads =impData[3];\n"
" long textLength =0;\n "
" patCount[gid]=0;\n"
" start = gid*divisionFactor;\n"
" textLength = (gid+1)*divisionFactor+patternLength-1;\n"
" if(gid == numberOfThreads)\n"
"    textLength = textLength1;\n"
/*
" if(gid!=numberOfThreads-1)\n"
	    "textLength = (gid+1)*(divisionFactor)+patternLength;\n"
" else \n"
"        textLength = textLength1;\n"
" start=gid*(divisionFactor) + 1;\n"
*/
"	for(;start<textLength;)\n"
"	{ \n"
	 " if((j<0)||(text[start]==pattern[j]))\n"
	 " { \n"
		"j++;\n"
		"start++;\n"
		"if(j==patternLength)\n"
		"{\n"
              "patCount[gid]++;\n"
		      " j=0;\n"
		 "}\n"
	  "}\n"
	  "else \n"
	  "{\n"
		  "j=Prefix[j];\n"
	  "}\n"
	"}\n"
"}\n";

//void SequentialKMP::searchWithNumberOfOpenCLWorkItem(long numberofWorkItem){
void SequentialKMP::searchWithNumberOfOpenCLWorkItem(){
    TextAndPatternManager textPatternObj(textFileName,patternFileName,SINGLE);
	textPatternObj.loadTextData();
	char* text = textPatternObj.getTextData();
	textPatternObj.loadPatternData();
	char *pattern = textPatternObj.getPatternData();
	long patternlength = strlen(pattern);
    calculatePrefixFunction(pattern);
	numberOfOccurence=0;

   cl_platform_id *platform;
	cl_uint num;
    clGetPlatformIDs(2,NULL,&num); 
    platform = (cl_platform_id*)malloc(num*sizeof(cl_platform_id)); 
	clGetPlatformIDs(2,platform,&num);
    cl_int err;
    cl_device_id device;
    cl_int m= clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, 1, &device,NULL); 
    cl_context context = clCreateContext(0, // optional (context properties)
                                          1, // number of devices
                                          &device, // pointer to device list
                                          NULL, NULL, // optional (callback function for reporting errors)
                                          NULL); // no error code returned
                               
	cl_command_queue queue = clCreateCommandQueue(context, // valid context
                                                   device, // device associated with context
                                                   CL_QUEUE_PROFILING_ENABLE, // optional (command queue properties)
                                                   NULL); // no error code returned
    cl_program program = clCreateProgramWithSource(context,
		                                           1, // number of strings
                                                   &source, // strings
                                                   NULL, // string length or NULL terminated
                                                   NULL); // no error code returned
    clBuildProgram(program,
                   1, // number of devices
                   &device, // pointer to device list
                   NULL, // optional (build options)
                   NULL, NULL); // optional (callback function, argument)

    cl_kernel kernel = clCreateKernel(program, // program object
                                      "KMPSearch", // kernel name in program
                                      NULL); // no error code returned
    long textLength = strlen(text);
	long numberofWorkItem;
	numberofWorkItem = calculateWorkItems(textLength,3*patternlength);
   	// Initialize arrays
    // for text and pattern kernal arguments
    cl_char *a = (cl_char *) malloc(textLength*sizeof(cl_char));
    cl_char *b = (cl_char *) malloc(patternlength*sizeof(cl_char));
    for(int i =0; i<textLength;i++)
	{
	   a[i]=text[i];
    }
      for(int i =0; i<patternlength;i++)
	{
	   b[i]=pattern[i];
    }
	// A buffer object is a handle to a region of memory
    cl_mem a_buffer = clCreateBuffer(context,
                                     CL_MEM_READ_ONLY | // buffer object read only for kernel
                                     CL_MEM_COPY_HOST_PTR, // copy data from memory referenced
									 // by host pointer
                                     textLength*sizeof(cl_char), // size in bytes of buffer object
                                     a, // host pointer
                                     NULL); // no error code returned
    cl_mem b_buffer = clCreateBuffer(context, 
		                             CL_MEM_READ_ONLY |CL_MEM_COPY_HOST_PTR,
                                     patternlength*sizeof(cl_char), b, NULL);
	
	
    // for pattern count argument
    cl_mem c_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                     numberofWorkItem*sizeof(cl_long), NULL, NULL);
    
	// for imp data arguments
    cl_long *impData = (cl_long *) malloc(4*sizeof(cl_long));
	impData[0]=textLength;
	impData[1]= patternlength;
	impData[2] = 3*patternlength;
    impData[3] = numberofWorkItem;

	cl_mem impData_buffer = clCreateBuffer(context, 
		                             CL_MEM_READ_ONLY |CL_MEM_COPY_HOST_PTR,
                                     4*sizeof(cl_long),impData, NULL);
	
	// for Prefix Array 
	cl_int *prefixArray1 = (cl_int *) malloc(patternlength*sizeof(cl_int));
	for(int i=0;i<patternlength;i++)
	{
	    prefixArray1[i]=PrefixArray[i];
	}

	cl_mem prefix_buffer = clCreateBuffer(context, 
		                             CL_MEM_READ_ONLY |CL_MEM_COPY_HOST_PTR,
                                     patternlength*sizeof(cl_int),prefixArray1, NULL);
	size_t global_work_size = numberofWorkItem;
    // Set the kernel arguments
    clSetKernelArg(kernel, 0, sizeof(a_buffer), (void*) &a_buffer);
    clSetKernelArg(kernel, 1, sizeof(b_buffer), (void*) &b_buffer);
  	clSetKernelArg(kernel, 2, sizeof(c_buffer), (void*) &c_buffer);
    clSetKernelArg(kernel, 3, sizeof(impData_buffer), (void*) &impData_buffer);
    clSetKernelArg(kernel, 4, sizeof(prefix_buffer), (void*) &prefix_buffer);


	cl_long *c = (cl_long *) malloc(numberofWorkItem*sizeof(cl_long));
	
	// Enqueue a command to execute the kernel on the GPU device
    //timeObj.start();
   cl_event timeEvent;
   cl_int error= clEnqueueNDRangeKernel(queue, kernel,
                           1, NULL, // global work items dimensions and offset
                           &global_work_size, // number of global work items
                           NULL, // number of work items in a work group
                           0, NULL, // don't wait on any events to complete
                            &timeEvent); // no event object returned

	// Block until all commands in command-queue have completed
    clFinish(queue);
    //timeObj.stop();
	// Read back the results
   
    clEnqueueReadBuffer(
                        queue, // command queue in which read command will be queued
                        c_buffer, // buffer object to read back
                        CL_TRUE, // blocking read - doesn't return until buffer copied
                        0, // offset in bytes in buffer object to read from
                        numberofWorkItem * sizeof(cl_long), // size in bytes of data being read
                        c, // pointer to host memory where data is to be read into
                        0, NULL, // don't wait on any events to complete
                        NULL); // no event object returned
   
	for(int i=0;i<numberofWorkItem;i++)
		numberOfOccurence = numberOfOccurence+c[i];//cout<<"\n"<<c[i];
    cl_ulong startBuf;
 // size_t a = 1000;
 //clWaitForEvents(1,&timeEvent);
  cl_int x= clGetEventProfilingInfo (  timeEvent,  
                           CL_PROFILING_COMMAND_START ,  
                           sizeof(cl_ulong),  
                           &startBuf,  
                          NULL);

  cl_ulong endBuf;
 // size_t a = 1000;
  cl_int y= clGetEventProfilingInfo (  timeEvent,  
                           CL_PROFILING_COMMAND_END ,  
                           sizeof(cl_ulong),  
                           &endBuf,  
                          NULL);
    long diff = endBuf - startBuf;
	double values_in_second = (double)diff/(double)1000000000;
	cout<<"Total GPU time:"<<values_in_second<<"\n";

	free(impData);
	free(prefixArray1);
	free(c);
	clReleaseMemObject(a_buffer);
    clReleaseMemObject(b_buffer);
    clReleaseMemObject(c_buffer);
	clReleaseMemObject(impData_buffer);
    clReleaseMemObject(prefix_buffer);
	clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseCommandQueue(queue);
	//getchar();
}
long SequentialKMP::calculateWorkItems(long textLength,long patternLength){
	long numberOfWorkItems=ceil(((float)textLength/(float)patternLength));
	return numberOfWorkItems;
}