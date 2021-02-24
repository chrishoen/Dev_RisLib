/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include <sys/resource.h>

#include "risThreadsProcess.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

static int mTimerPeriod = 10;

void setProcessTimerResolution(int aTimerPeriod)
{
   mTimerPeriod = aTimerPeriod;
}

int  getProcessTimerResolution()
{
   return mTimerPeriod;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void enterProcessHigh()
{
   int tRet = 0;
   int tMaxPriority = sched_get_priority_max(SCHED_FIFO);
   int tMinPriority = sched_get_priority_min(SCHED_FIFO);

   struct rlimit tRLimit;
   tRLimit.rlim_cur = tMinPriority;
   tRLimit.rlim_max = tMaxPriority;
   tRet = setrlimit(RLIMIT_RTPRIO, &tRLimit);
   if (tRet)
   {
      printf("setrlimit ERROR  %d\n", errno);
   }
}

void exitProcess()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void setProcessAffinityMask(unsigned aMask)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void showCurrentThreadInfo()
{
   int tCurrentProcessorNumber = sched_getcpu();;

   sched_param param;
   int policy;
   pthread_getschedparam(pthread_self(), &policy, &param);
   int tThreadPriority = param.sched_priority;

   TS::print(1, "");
   TS::print(0, "ThreadInfo %-20s %1d %3d %-8s %1d",
      "main",
      tCurrentProcessorNumber,
      tThreadPriority,
      "running",
      TS::tls()->mPrintLevel);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

