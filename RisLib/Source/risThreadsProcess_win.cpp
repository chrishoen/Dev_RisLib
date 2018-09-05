//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h> 

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
   // Restore the  process timer resolution.
   timeEndPeriod(mTimerPeriod);
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
}//namespace
}//namespace

#if 0
int tPriorityClass = GetPriorityClass(GetCurrentProcess());
printf("GetPriorityClass %08X\n", tPriorityClass);
#endif
