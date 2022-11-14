/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

//#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <functional>
#include <sys/resource.h>

#include "my_functions.h"
#include "risThreadsPriorities.h"
#include "prnPrint.h"
#include "risThreadsThreads.h"


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

static void* BaseThread_Execute (void* argument)
{
   BaseThread* someThread = (BaseThread*)argument;
   someThread->threadFunction();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a c-function that is called if a thread is cancelled. It calls
// the thread exit function.

void BaseThread_Cleanup(void* argument)
{
   BaseThread* someThread = (BaseThread*)argument;
   someThread->threadExitFunction();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BaseThread::BaseSpecific
{
public:
   pthread_t mHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

BaseThread::BaseThread() 
{
   mBaseSpecific = new BaseSpecific;
   mBaseSpecific->mHandle = 0;
   strcpy(mThreadName, "noname");
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
// Set the thread name.

void BaseThread::setThreadName(const char* aThreadName)
{
   strcpy(mThreadName, aThreadName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread processor number and priority.

void BaseThread::setThreadPriority(Priority aPriority)
{
   mThreadSingleProcessor = aPriority.mProcessor;
   mThreadPriority        = aPriority.mPriority;
}

void BaseThread::setThreadPriority(int aPriority)
{
   mThreadSingleProcessor = -1;
   mThreadPriority = aPriority;
}

void BaseThread::setThreadPriorityLow()
{
   mThreadSingleProcessor = gPriorities.mLow.mProcessor;
   mThreadPriority        = gPriorities.mLow.mPriority;
}

void BaseThread::setThreadPriorityNormal()
{
   mThreadSingleProcessor = gPriorities.mNormal.mProcessor;
   mThreadPriority        = gPriorities.mNormal.mPriority;
}

void BaseThread::setThreadPriorityHigh()
{
   mThreadSingleProcessor = gPriorities.mHigh.mProcessor;
   mThreadPriority        = gPriorities.mHigh.mPriority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

static void chkerror(int aRet, const char* aLabel)
{
   if (aRet == 0)return;
   printf("PTHREAD FAIL %s %d\n", aLabel,aRet);
   exit(1);
}

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
   // Thread attributes.

   // Thread attributes, initialize.
   pthread_attr_t tAttributes;
   pthread_attr_init(&tAttributes);
   
   int ret;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes, thread priority.

   ret = pthread_attr_setschedpolicy(&tAttributes, SCHED_FIFO);
   chkerror(ret, "pthread_attr_setschedpolicy");

   sched_param tSchedParam;
   tSchedParam.sched_priority = mThreadPriority;
   ret = pthread_attr_setschedparam(&tAttributes, &tSchedParam);
   chkerror(ret, "pthread_attr_setschedparam");

   ret = pthread_attr_setinheritsched(&tAttributes, PTHREAD_EXPLICIT_SCHED);
   chkerror(ret, "pthread_attr_setinheritsched");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes, affinity mask.

   if (mThreadSingleProcessor >= 0)
   {
      cpu_set_t tAffinityMask;
      CPU_ZERO(&tAffinityMask);
      CPU_SET(mThreadSingleProcessor, &tAffinityMask);
      ret = pthread_attr_setaffinity_np(&tAttributes, sizeof(tAffinityMask), &tAffinityMask);
      chkerror(ret, "pthread_attr_setaffinity_np");
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Create the thread. The following threadRunFunction will execute
   // in the context of the created thread.

   ret = pthread_create(
      &mBaseSpecific->mHandle,
      &tAttributes,
      &BaseThread_Execute,
      (void*)this);
   chkerror(ret, "pthread_create");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes, finalize.

   pthread_attr_destroy(&tAttributes);

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

   // Push the thread cleanup function. This is called if a thread is 
   // cancelled. It calls the thread exit function.
   pthread_cleanup_push(BaseThread_Cleanup, this);

   // Thread execution
   try
   {
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

   // Pop the thread cleanup function without executing it. This is the
   // normal execution path, where the thread is cancelled. If the thread
   // is cancelled, this does not execute.
   pthread_cleanup_pop(0);

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
   // Calculate timespec variables
   int tMs  = aTicks;
   int tSec = tMs / 1000;
   int tNs  = (tMs - tSec*1000)*1000000;
   if (tNs>999999999) tNs=999999999;

   timespec tTime;
   tTime.tv_sec=tSec;
   tTime.tv_nsec = tNs;

   // Nanosleep
   nanosleep(&tTime,&tTime);
 //sleep(aTicks);  
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::forceTerminateThread()
{
   pthread_cancel(mBaseSpecific->mHandle);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::waitForThreadTerminate()
{
   pthread_join(mBaseSpecific->mHandle,NULL);
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

int BaseThread::getThreadPriority()
{
   sched_param param;
   int policy;

   pthread_getschedparam(mBaseSpecific->mHandle, &policy, &param);

   return param.sched_priority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BaseThread::getThreadProcessorNumber()
{
   mThreadCurrentProcessor = sched_getcpu();
   return mThreadCurrentProcessor;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BaseThread::showThreadFullInfo()
{
   printf("ThreadInfo>>>>>>>>>>>>>>>>>>>>>>>>>>BEGIN %s\n", mThreadName);

   int tMaxPriority = sched_get_priority_max(SCHED_FIFO);
   int tMinPriority = sched_get_priority_min(SCHED_FIFO);

   sched_param tThreadSchedParam;
   int tThreadPolicy = 0;
   pthread_getschedparam(mBaseSpecific->mHandle, &tThreadPolicy, &tThreadSchedParam);
   int tThreadPriority = tThreadSchedParam.__sched_priority;

   int tNumProcessors = sysconf(_SC_NPROCESSORS_ONLN);
   int tCurrentProcessorNumber = sched_getcpu();

   cpu_set_t tAffinityMask;
   pthread_getaffinity_np(mBaseSpecific->mHandle, sizeof(tAffinityMask), &tAffinityMask);
   unsigned tUMask = 0;
   for (int i = 0; i < 32; i++) if (CPU_ISSET(i, &tAffinityMask)) (tUMask |= (1 << i));

   struct rlimit tRLimit;
   getrlimit(RLIMIT_RTPRIO, &tRLimit);
   int tRTPriority = tRLimit.rlim_cur;
   int tRRTMaxPriority = tRLimit.rlim_max;

   printf("NumProcessors           %8d\n", tNumProcessors);
   printf("MaxPriority             %8d\n", tMaxPriority);
   printf("MinPriority             %8d\n", tMinPriority);
   printf("\n");
   printf("ThreadPolicy            %8d\n", tThreadPolicy);
   printf("ThreadPriority          %8d\n", tThreadPriority);
   printf("\n");
   printf("ThreadAffinityMask      %8X\n", tUMask);
   printf("mThreadSingleProcessor  %8X\n", mThreadSingleProcessor);
   printf("CurrentProcessorNumber  %8d\n", tCurrentProcessorNumber);

   printf("RTPriority              %8d\n", tRTPriority);
   printf("RRTMaxPriority          %8d\n", tRRTMaxPriority);

   printf("ThreadInfo<<<<<<<<<<<<<<<<<<<<<<<<<<END\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BaseThread::showThreadInfo()
{
   int tThreadPriority = getThreadPriority();

   printf("ThreadInfo %-20s %2d %3d %-8s\n",
      mThreadName,
      mThreadRunProcessor,
      tThreadPriority,
      asStringThreadRunState());
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
   // Calculate timespec variables
   int tMs = aTicks;
   int tSec = tMs / 1000;
   int tNs = (tMs - tSec * 1000) * 1000000;
   if (tNs > 999999999) tNs = 999999999;

   timespec tTime;
   tTime.tv_sec = tSec;
   tTime.tv_nsec = tNs;

   // Nanosleep
   nanosleep(&tTime, &tTime);
   //sleep(aTicks);  
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void halt(const char* aPrintStr)
{
   printf("HALTING %s\n", aPrintStr);
   exit(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
