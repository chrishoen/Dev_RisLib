/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <windows.h>

#define  _RISTHREADSPRIORITIES_CPP_
#include "risThreadsPriorities.h"

namespace Ris
{
namespace Threads
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
/*
   THREAD_PRIORITY_TIME_CRITICAL;
   THREAD_PRIORITY_HIGHEST;
   THREAD_PRIORITY_ABOVE_NORMAL;
   THREAD_PRIORITY_NORMAL;
   THREAD_PRIORITY_BELOW_NORMAL;
   THREAD_PRIORITY_LOWEST;
   THREAD_PRIORITY_IDLE;
   */

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Priority::Priority()
{
   mProcessor = -1;
   mPriority = 50;
}

Priority::Priority(short aProcessor, short aPriority)
{
   mProcessor = aProcessor;
   mPriority = aPriority;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

Priorities::Priorities()
{
   mLow       = Priority(-1, THREAD_PRIORITY_LOWEST);
   mNormal    = Priority(-1, THREAD_PRIORITY_NORMAL);
   mHigh      = Priority(-1, THREAD_PRIORITY_TIME_CRITICAL);

   mQCall     = Priority(-1, THREAD_PRIORITY_NORMAL);
   mShort     = Priority(-1, THREAD_PRIORITY_TIME_CRITICAL);
   mLong      = Priority(-1, THREAD_PRIORITY_NORMAL);

   mUdp       = Priority(-1, THREAD_PRIORITY_TIME_CRITICAL);
   mTcpServer = Priority(-1, THREAD_PRIORITY_NORMAL);
   mTcpClient = Priority(-1, THREAD_PRIORITY_NORMAL);
   mSerial    = Priority(-1, THREAD_PRIORITY_TIME_CRITICAL);

   mTsPrint   = Priority(-1, THREAD_PRIORITY_LOWEST);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
