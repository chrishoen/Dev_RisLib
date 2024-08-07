/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risBtSettings.h"

namespace Ris
{
namespace Bt
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Settings::Settings()
{
   mRemoteName[0] = 0;
   mMaxSessions = 0;
   mTraceIndex = 0;
   mFlags = 0;
   mMsgMonkey = 0;
   mThreadPriority = Ris::Threads::gPriorities.mUdp;
   mMaxDataSize = 20000;
}

void Settings::setRemoteName(const char* aRemoteName)
{
   strcpy(mRemoteName, aRemoteName);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

