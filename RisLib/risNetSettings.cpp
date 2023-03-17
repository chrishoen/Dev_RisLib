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
   mLocalIpAddr[0] = 0;
   mLocalIpPort = 0;
   mRemoteIpAddr[0] = 0;
   mRemoteIpPort = 0;
   mTestForLocal = false;
   mUdpWrapFlag = false;
   mUdpBroadcast = false;
   mMaxSessions = 0;
   mTraceIndex = 0;
   mFlags = 0;
   mMsgMonkey = 0;
   mThreadPriority = Ris::Threads::gPriorities.mUdp;
   mMaxDataSize = 1500;
}

void Settings::setLocalAddress(const char* aIpAddr, int aIpPort)
{
   strcpy(mLocalIpAddr, aIpAddr);
   mLocalIpPort = aIpPort;
}

void Settings::setRemoteAddress(const char* aIpAddr, int aIpPort)
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

// Set member.
void Settings::setUdpBroadcast(bool aUdpBroadcast)
{
   mUdpBroadcast = aUdpBroadcast;
   if (!mUdpBroadcast) return;
   strcpy(mRemoteIpAddr, "UdpBroadcast");
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

