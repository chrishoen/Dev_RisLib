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
namespace Bt
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

   // Transmit to this address and port.
   char mRemoteName[100];

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

   void setRemoteName(const char* aRemoteName);

   // Constructor.
   Settings();
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

