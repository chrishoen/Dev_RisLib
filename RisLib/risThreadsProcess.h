#pragma once

/*==============================================================================
==============================================================================*/

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
// These configure process properties.

void setProcessTimerResolution(int aTimerPeriod);
int  getProcessTimerResolution();

void enterProcessHigh();
void exitProcess();

void setProcessAffinityMask(unsigned aMask);

void showCurrentThreadInfo();

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These handle signals.

// Post to the termination semaphore.
void postTermination();

// Block on the termination semaphore. This can be posted to by the above
// call or by a kill signal.
void waitForTermination();

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


