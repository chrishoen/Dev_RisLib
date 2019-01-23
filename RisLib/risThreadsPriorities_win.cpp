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

   static const int tInt1 = THREAD_PRIORITY_TIME_CRITICAL;
   static const int tInt2 = THREAD_PRIORITY_HIGHEST;
   static const int tInt3 = THREAD_PRIORITY_ABOVE_NORMAL;
   static const int tInt4 = THREAD_PRIORITY_NORMAL;
   static const int tInt5 = THREAD_PRIORITY_BELOW_NORMAL;
   static const int tInt6 = THREAD_PRIORITY_LOWEST;
   static const int tInt7 = THREAD_PRIORITY_IDLE;


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
   mLong      = Priority(-1, THREAD_PRIORITY_HIGHEST);

   mUdp       = Priority(-1, THREAD_PRIORITY_TIME_CRITICAL);
   mTcpServer = Priority(-1, THREAD_PRIORITY_NORMAL);
   mTcpClient = Priority(-1, THREAD_PRIORITY_NORMAL);
   mSerial    = Priority(-1, THREAD_PRIORITY_TIME_CRITICAL);

   mTsPrint   = Priority(-1, THREAD_PRIORITY_ABOVE_NORMAL);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
