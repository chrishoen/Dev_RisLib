#pragma once

/*==============================================================================
Byte content message network socket settings class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risThreadsQCall.h"
#include "risThreadsPriorities.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates network socket settings. They are used to configure
// the various socket classes.

class Settings
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Receive on this address and port.
   char mLocalIpAddr[20];
   int mLocalIpPort;

   // Transmit to this address and port.
   char mRemoteIpAddr[20];
   int  mRemoteIpPort;

   // If true then test that the remote address is local.
   bool mTestForLocal;

   // If true then use the first receive from ip address as the
   // transmit ip address. Don't open the transmit socket until
   // a message is received.
   bool mUdpWrapFlag;

   // If true then broadcast udp sends.
   bool mUdpBroadcast;

   // Max number of tcp server sessions.
   int mMaxSessions;

   // Socket flags.
   int mFlags;

   // Program trace index.
   int mTraceIndex;

   // Thread processor number and priority.
   Ris::Threads::Priority mThreadPriority;

   // Message monkey.
   BaseMsgMonkey* mMsgMonkey;

   // Session callback qcall.
   Ris::Threads::QCall1<bool>     mClientSessionQCall;
   Ris::Threads::QCall2<int,bool> mServerSessionQCall;

   // Receive byte content message callback qcall.
   Ris::Threads::QCall1<Ris::ByteContent*> mRxMsgQCall;
   Ris::Threads::QCall2<int,Ris::ByteContent*> mServerRxMsgQCall;

   // Receive string callback qcall.
   Ris::Threads::QCall1 <std::string*> mRxStringQCall;

   // Receive data callback qcall.
   Ris::Threads::QCall2 <char*,int> mRxDataQCall;

   // Maximum receive data size.
   int mMaxDataSize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Settings();

   // Set member.
   void setLocalAddress (const char* aIpAddr, int aIpPort);
   void setRemoteAddress(const char* aIpAddr, int aIpPort);

   // Set member.
   void setUdpWrapFlag(bool aUdpWrapFlag);
   void setUdpBroadcast(bool aUdpBroadcast);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

