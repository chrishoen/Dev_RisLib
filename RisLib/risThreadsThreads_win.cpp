/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 
#include <assert.h>

#include "my_functions.h"
#include "risThreadsPriorities.h"
#include "prnPrint.h"
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a c-function that is used to execute the thread function of 
// a base thread object. It is passed to CreateThread to indirectly call
// the thread function because CreateThread can't use member function
// addresses.

int BaseThread_Execute (BaseThread* someThread)
{
   someThread->threadFunction();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseThread::BaseSpecific
{
public:
   HANDLE mHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::BaseThread()
{
   mBaseSpecific = new BaseSpecific;
   mBaseSpecific->mHandle = 0;
   mThreadRunState = 0;
   mThreadPriority = gPriorities.mNormal.mPriority;
   mThreadSingleProcessor = -1;
   mThreadStackSize = 0;
   mThreadRunProcessor = -1;
   mThreadCurrentProcessor = 0;
   mTerminateFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::~BaseThread() 
{
   delete mBaseSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread services thread name in the thread local storage.

void BaseThread::setThreadName(const char* aThreadName)
{
   strcpy(mThreadName, aThreadName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::setThreadPriority(Priority aPriority)
{
   mThreadSingleProcessor = aPriority.mProcessor;
   mThreadPriority = aPriority.mPriority;
}

void BaseThread::setThreadPriority(int aPriority)
{
   mThreadSingleProcessor = -1;
   mThreadPriority = aPriority;
}

void BaseThread::setThreadPriorityLow()
{
   mThreadSingleProcessor = gPriorities.mLow.mProcessor;
   mThreadPriority = gPriorities.mLow.mPriority;
}

void BaseThread::setThreadPriorityNormal()
{
   mThreadSingleProcessor = gPriorities.mNormal.mProcessor;
   mThreadPriority = gPriorities.mNormal.mPriority;
}

void BaseThread::setThreadPriorityHigh()
{
   mThreadSingleProcessor = gPriorities.mHigh.mProcessor;
   mThreadPriority = gPriorities.mHigh.mPriority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::launchThread()
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Do this first.

   // Set the run state.
   mThreadRunState = cThreadRunState_Launching;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create thread, suspended.

   DWORD tThreadFlags = CREATE_SUSPENDED;

   // Create the thread in a suspended state.
   // The following threadFunction() is executed in 
   // the context of the created thread.
   mBaseSpecific->mHandle=CreateThread(
        NULL, 
       (DWORD) mThreadStackSize,
       (LPTHREAD_START_ROUTINE)BaseThread_Execute,
       (LPVOID)this,
       (DWORD)tThreadFlags,
        NULL);
 
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Set thread parameters.

   if( SetThreadPriority(mBaseSpecific->mHandle,mThreadPriority) == 0 )
   {
      char tStr[100];
      sprintf(tStr, "Bad Thread Priority %d %d %d",mThreadPriority,*((int*)&mBaseSpecific->mHandle),(int)GetLastError());
      halt(tStr);
   }

   if (mThreadSingleProcessor != -1)
   {
      int tAffinityMask = 1 << mThreadSingleProcessor;
      if( SetThreadAffinityMask(mBaseSpecific->mHandle,tAffinityMask) == -1 ) throw "Bad Thread Affinity";
      if( SetThreadIdealProcessor(mBaseSpecific->mHandle,mThreadSingleProcessor) == -1 ) throw "Bad Ideal Processor";
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Resume thread, this starts the thread function within the context of
   // the new thread,using configured priority and affinity.

   ResumeThread(mBaseSpecific->mHandle);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Wait for the thread init function to complete.

   mThreadInitSem.get();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::threadFunction()
{
   // Set the processor that was current at the start of the thread
   // run function.
   mThreadRunProcessor = getThreadProcessorNumber();

   // Thread execution
   try
   {
      // Seed thread random number.
      my_srand();
      // This is used by inheritors to initialize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      mThreadRunState = cThreadRunState_InitR;
      threadResourceInitFunction();
      // Initialization section, overload provided by inheritors
      // It is intended that this will be overloaded by 
      // inheriting thread base classes and by inheriting user classes
      mThreadRunState = cThreadRunState_InitF;
      threadInitFunction();
      // Post to the thread init semaphore.
      mThreadInitSem.put();
      // Run section, overload provided by inheritors 
      mThreadRunState = cThreadRunState_Running;
      threadRunFunction();
      // Exit section, overload provided by inheritors
      mThreadRunState = cThreadRunState_ExitF;
      threadExitFunction();
      // This is used by inheritors to finalize resources. This should be
      // overloaded by thread base classes and not by thread user classes.
      mThreadRunState = cThreadRunState_ExitR;
      threadResourceExitFunction();
   }
   catch (char* aStr)
   {
      // Exception section, overload provided by inheritors
      threadExceptionFunction(aStr);
   }
   catch (...)
   {
      // Exception section, overload provided by inheritors
      threadExceptionFunction("UNKNOWN");
   }

   // Set the run state.
   mThreadRunState = cThreadRunState_Terminated;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::threadExceptionFunction(const char* aStr)
{
   Prn::print(0,"BaseThread::threadExceptionFunction %s",aStr);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::threadSleep(int aTicks)
{
   Sleep(aTicks);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::forceTerminateThread()
{
   TerminateThread(mBaseSpecific->mHandle,-1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::waitForThreadTerminate()
{
   WaitForSingleObject(mBaseSpecific->mHandle,INFINITE);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::shutdownThread()
{
   mTerminateFlag = true;
   waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int countsPerOneSecond()
{
   return 1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BaseThread::getThreadPriority()
{
   return GetThreadPriority(mBaseSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BaseThread::getThreadProcessorNumber()
{
   mThreadCurrentProcessor = GetCurrentProcessorNumber();
   return mThreadCurrentProcessor;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::showThreadFullInfo()
{
   printf("ThreadInfo>>>>>>>>>>>>>>>>>>>>>>>>>>BEGIN %s\n", mThreadName);

   unsigned tPriorityClass = GetPriorityClass(GetCurrentProcess());
   int tThreadPriority = GetThreadPriority(mBaseSpecific->mHandle);
   int tCurrentProcessorNumber = GetCurrentProcessorNumber();

   DWORD_PTR tProcessAffinityMask = 0;
   DWORD_PTR tSystemAffinityMask = 0;

   GetProcessAffinityMask(
      GetCurrentProcess(),
      &tProcessAffinityMask,
      &tSystemAffinityMask);

   printf("PriorityClass           %8d\n", tPriorityClass);
   printf("ThreadPriority          %8d\n", tThreadPriority);
   printf("SystemAffinityMask      %8X\n", (unsigned)tSystemAffinityMask);
   printf("ProcessAffinityMask     %8X\n", (unsigned)tProcessAffinityMask);
   printf("SingleProcessor         %8d\n", mThreadSingleProcessor);
   printf("CurrentProcessorNumber  %8d\n", tCurrentProcessorNumber);

   printf("ThreadInfo<<<<<<<<<<<<<<<<<<<<<<<<<<END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::showThreadInfo()
{
   int tThreadPriority = getThreadPriority();

   printf("ThreadInfo %-20s %1d %3d %-8s\n",
      mThreadName,
      mThreadRunProcessor,
      tThreadPriority,
      asStringThreadRunState());
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

const char* BaseThread::asStringThreadRunState()
{
   switch (mThreadRunState)
   {
   case cThreadRunState_Launching:  return  "launching";
   case cThreadRunState_InitR:      return  "initR";
   case cThreadRunState_InitF:      return  "initF";
   case cThreadRunState_Running:    return  "running";
   case cThreadRunState_ExitF:      return  "exitF";
   case cThreadRunState_ExitR:      return  "exitR";
   case cThreadRunState_Terminated: return  "termed";
   default: return "UNKNOWN";
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void threadSleep(int aTicks)
{
   Sleep(aTicks);
}

void halt(const char* aPrintStr)
{
   printf("HALTING %s\n",aPrintStr);
   exit(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

