/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#define  _RISTHREADSPRIORITIES_CPP_
#include "risThreadsPriorities.h"

namespace Ris
{
namespace Threads
{

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
   mLow       = Priority(-1, 20);
   mNormal    = Priority(-1, 50);
   mHigh      = Priority(-1, 80);

   mQCall     = Priority(-1, 50);
   mShort     = Priority(-1, 80);
   mLong      = Priority(-1, 50);

   mUdp       = Priority(-1, 80);
   mTcpServer = Priority(-1, 50);
   mTcpClient = Priority(-1, 50);
   mSerial    = Priority(-1, 80);
   mTimerTest = Priority( 3, 95);
   mTimerTest = Priority( 2, 95);

   mTsPrint   = Priority(-1, 20);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace
