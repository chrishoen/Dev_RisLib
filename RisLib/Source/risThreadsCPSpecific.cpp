/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsCPSpecific.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor

CPSpecificQCallThread::CPSpecificQCallThread()
{
}

CPSpecificQCallThread::~CPSpecificQCallThread()
{
}

//******************************************************************************
// BaseClass overload

void CPSpecificQCallThread::threadInitFunction() 
{
   // Call the call pointer
   if (mThreadInitCallPointer.isValid()) mThreadInitCallPointer();
}

//******************************************************************************
// BaseClass overload

void CPSpecificQCallThread::threadExitFunction() 
{
   // Call the call pointer
   if (mThreadExitCallPointer.isValid()) mThreadExitCallPointer();
}

//******************************************************************************
// BaseClass overload

void CPSpecificQCallThread::threadExceptionFunction(char* aStr)
{
   // Call the call pointer
   if (mThreadExceptionCallPointer.isValid()) mThreadExceptionCallPointer(aStr);
}

//******************************************************************************
// BaseClass overload

void CPSpecificQCallThread::executeOnTimer(int aTimerCount)
{
   // Call the call pointer
   if (mThreadExecuteOnTimerCallPointer.isValid()) mThreadExecuteOnTimerCallPointer(aTimerCount);
}

}//namespace
}//namespace

