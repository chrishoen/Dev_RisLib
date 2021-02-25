/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

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
#include "prnPrint.h"
#include "risThreadsPriorities.h"
#include "risNanoConvert.h"
#include "risThreadsPeriodicThread2.h"

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

static void* BasePeriodicThread2_Execute (void* argument)
{
   BasePeriodicThread2* someThread = (BasePeriodicThread2*)argument;
   someThread->threadFunction();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

class BasePeriodicThread2::BaseSpecific
{
public:
   pthread_t mHandle;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************

BasePeriodicThread2::BasePeriodicThread2() 
{
   mBaseSpecific = new BaseSpecific;
   mBaseSpecific->mHandle = 0;
   mThreadPriority = gPriorities.mNormal.mPriority;
   mThreadSingleProcessor = -1;
   mThreadStackSize = 0;
   mThreadRunProcessor = -1;
   mTimerPeriod = 1000;
   mTimerCount = 0;
   mTerminateFlag = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

BasePeriodicThread2::~BasePeriodicThread2() 
{
   delete mBaseSpecific;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Set the thread processor number and priority.

void BasePeriodicThread2::setThreadPriority(Priority aPriority)
{
   mThreadSingleProcessor = aPriority.mProcessor;
   mThreadPriority        = aPriority.mPriority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

static void chkerror(int aRet, const char* aLabel)
{
   if (aRet >= 0)return;
   printf("FAIL %s %d\n", aLabel,aRet);
   exit(1);
}

void BasePeriodicThread2::launchThread()
{
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
      &BasePeriodicThread2_Execute,
      (void*)this);
   chkerror(ret, "pthread_create");

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Thread attributes, finalize.

   pthread_attr_destroy(&tAttributes);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BasePeriodicThread2::threadFunction()
{
   // Set the processor that was current at the start of the thread
   // run function.
   mThreadRunProcessor = getThreadProcessorNumber();

   // Time variables
   timespec   tSleepTimespec;
   long long  tSleepTimeNs;
   long long  tTimerPeriodNs = Ris::NanoConvert::getNsFromMs(mTimerPeriod);

   // Get current timespec at start.
   clock_gettime(CLOCK_MONOTONIC, &tSleepTimespec);

   // Convert to ns.
   Ris::NanoConvert::getNsFromTimespec(tSleepTimeNs, &tSleepTimespec);

   // Loop until thread terminate.
   while (!mTerminateFlag)
   {
      // Advance the sleep time by the period.
      tSleepTimeNs += tTimerPeriodNs;

      // Convert to timespec.
      Ris::NanoConvert::getTimespecFromNs(&tSleepTimespec, tSleepTimeNs);

      // Sleep until the absolute sleep time.
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tSleepTimespec, 0);

      // Call the inheritors handler for the timer.
      executeOnTimer(mTimerCount++);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BasePeriodicThread2::waitForThreadTerminate()
{
   pthread_join(mBaseSpecific->mHandle,NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void BasePeriodicThread2::shutdownThread()
{
   mTerminateFlag = true;
   pthread_join(mBaseSpecific->mHandle, NULL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

int BasePeriodicThread2::getThreadProcessorNumber()
{
   return sched_getcpu();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is the function that is executed in the context of the created thread.
// It calls a sequence of functions that are overloaded by inheritors.

void BasePeriodicThread2::showThreadFullInfo()
{
   printf("ThreadInfo>>>>>>>>>>>>>>>>>>>>>>>>>>BEGIN\n");

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
}//namespace
}//namespace
