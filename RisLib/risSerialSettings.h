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

// Serial string termination modes.
static const int cSerialTermMode_NULL = 0; // Null terminated.
static const int cSerialTermMode_LF   = 1; // LF   (\n,10)
static const int cSerialTermMode_CR   = 2; // CR   (\r,13)
static const int cSerialTermMode_CRLF = 3; // CRLF (\r\n,13,10)

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
   char mPortDevice[200];

   // "9600,N,8,1". If empty string then use system defaults.
   char mPortSetup[16];

   // Send and receive timeouts. Milliseconds, 0 means infinite timeout.
   int mTxTimeout;
   int mRxTimeout;

   // If true then delete a message after sending it.
   bool mTxDeleteAfterSend;

   // Termination modes. LF (\n,10) or CRLF (\r\n,13,10).
   int mTxTermMode;
   int mRxTermMode;

   // Program trace index.
   int mTraceIndex;

   // If true then use RS485.
   bool m485Flag;

   // If true then use bluetooth.
   bool mBthFlag;

   // Thread processor number and priority.
   Ris::Threads::Priority mThreadPriority;

   // Message monkey.
   BaseMsgMonkey* mMsgMonkey;

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

   // Port config.
   void setPortDevice(const char* aPortDevice);
   void setPortSetup(const char* aPortSetup);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Helpers. Serial port settings termination mode strings.

int int_from_string_SerialSettingsTermMode(const char* aTermMode);
const char* string_from_int_SerialSettingsTermMode(int aTermMode);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

