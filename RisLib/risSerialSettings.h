#pragma once

/*==============================================================================
Byte content message serial port class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <string>
#include <functional>

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

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constants.
   
static const int cSerialTermMode_LF   = 0;
static const int cSerialTermMode_CRLF = 1;

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encapsulates serial port settings. They are used to configure
// the various serial port classes.

class SerialSettings
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Identifier.
   int mIdent;

   // "COM1"
   char mPortDevice[16];

   // "9600,N,8,1". If empty string then use system defaults.
   char mPortSetup[16];

   // Receive timeout. Milliseconds, 0==no timeout
   int mRxTimeout;

   // Termination mode. LF (\n,10) or CRLF (\r\n,13,10).
   int mTermMode;

   // Termination mode. LF (\n,10) or CRLF (\r\n,13,10).
   bool m485Flag;

   // Thread processor number and priority.
   Ris::Threads::Priority mThreadPriority;

   // Message monkey creator.
   BaseMsgMonkeyCreator* mMonkeyCreator;

   // Receive byte content message callback qcall.
   Ris::Threads::QCall1<Ris::ByteContent*> mRxMsgQCall;

   // Receive string callback qcall.
   Ris::Threads::QCall1<std::string*> mRxStringQCall;

   // Receive string callback function.
   std::function<void(std::string*)> mRxStringCallback;

   // Session callback qcall.
   Ris::Threads::QCall1<bool> mSessionQCall;

   // Session callback function.
   std::function<void(bool)> mSessionCallback;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   SerialSettings();
   void reset();

   void setPortDevice(char* aPortDevice);
   void setPortSetup(char* aPortSetup);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

