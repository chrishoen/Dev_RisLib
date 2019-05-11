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
#include "tsDefs.h"
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

   // Receive on this port.
   int mLocalIpPort;

   // Transmit to this address and port.
   char mRemoteIpAddr[20];
   int  mRemoteIpPort;

   // If true then use the first receive from ip address as the
   // transmit ip address. Don't open the transmit socket until
   // a message is received.
   bool mUdpWrapFlag;

   // Max number of tcp server sessions.
   int mMaxSessions;

   // Socket flags.
   int mFlags;

   // Thread processor number and priority.
   Ris::Threads::Priority mThreadPriority;

   // Thread print and log levels.
   TS::PrintLevel mPrintLevel;

   // Message monkey creator.
   BaseMsgMonkeyCreator* mMonkeyCreator;

   // Session callback qcall.
   Ris::Threads::QCall1<bool>     mClientSessionQCall;
   Ris::Threads::QCall2<int,bool> mServerSessionQCall;

   // Receive byte content message callback qcall.
   Ris::Threads::QCall1<Ris::ByteContent*> mRxMsgQCall;
   Ris::Threads::QCall2<int,Ris::ByteContent*> mServerRxMsgQCall;

   // Receive string callback qcall.
   Ris::Threads::QCall1 < std::string *> mRxStringQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   Settings();

   // Set member.
   void setLocalPort (int aIpPort);
   void setRemoteAddress(char* aIpAddr, int aIpPort);

   // Set member.
   void setUdpWrapFlag(bool aUdpWrapFlag);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

