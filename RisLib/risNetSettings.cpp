/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risNetSettings.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

Settings::Settings()
{
   mLocalIpPort = 0;
   mRemoteIpAddr[0] = 0;
   mRemoteIpPort = 0;
   mUdpWrapFlag = false;
   mMaxSessions = 0;
   mFlags = 0;
   mMonkeyCreator = 0;
   mThreadPriority = Ris::Threads::gPriorities.mUdp;
}

void Settings::setLocalPort(int aIpPort)
{
   mLocalIpPort = aIpPort;
}

void Settings::setRemoteAddress(char* aIpAddr, int aIpPort)
{
   strcpy(mRemoteIpAddr, aIpAddr);
   mRemoteIpPort = aIpPort;
}

// Set member.
void Settings::setUdpWrapFlag(bool aUdpWrapFlag)
{
   mUdpWrapFlag = aUdpWrapFlag;
   if (!mUdpWrapFlag) return;
   strcpy(mRemoteIpAddr, "UdpWrap");
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

