//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

#include "risThreadsSynch.h"
#include "risThreadsProcess.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

static int mTimerPeriod = 1;

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
   // Set process priority class.
   int status = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

   // Set process timer resolution.
   timeBeginPeriod(mTimerPeriod);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void exitProcess()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void setProcessAffinityMask(unsigned aMask)
{
   // Convert.
   DWORD_PTR tMask = (DWORD_PTR)aMask;

   // Set the process affinity mask.
   SetProcessAffinityMask(GetCurrentProcess(), aMask);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void showCurrentThreadInfo()
{
   unsigned tPriorityClass = GetPriorityClass(GetCurrentProcess());
   int tThreadPriority = GetThreadPriority(GetCurrentThread());
   int tCurrentProcessorNumber = GetCurrentProcessorNumber();

   printf("ThreadInfo %-20s %1d %3d %-8s $  %3d\n",
      "main",
      tCurrentProcessorNumber,
      tThreadPriority,
      "running",
      tPriorityClass);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

Ris::Threads::BinarySemaphore rTerminateSem;

void sig_handler(int signum)
{
   // Post a program termination request.
   printf("sig_handler %d\n", signum);
   rTerminateSem.put();
}

void waitForTermination()
{
   printf("WAIT FOR TERMINATE\n");
   rTerminateSem.get();
   printf("TERMINATING\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

