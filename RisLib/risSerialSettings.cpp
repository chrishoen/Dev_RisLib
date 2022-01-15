/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risSerialSettings.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialSettings::SerialSettings()
{
   reset();
}

void SerialSettings::reset()
{
   mIdent = -1;
   mPortDevice[0] = 0;
   mPortSetup[0] = 0;
   mRxTimeout = 0;
   mTermMode = 0;
   m485Flag = false;
   mMonkeyCreator = 0;
   mThreadPriority = Ris::Threads::gPriorities.mSerial;
   mRxStringCallback = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Port config.

void SerialSettings::setPortDevice(const char* aPortDevice)
{
   strcpy(mPortDevice, aPortDevice);
}

void SerialSettings::setPortSetup(const char* aPortSetup)
{
   strcpy(mPortSetup, aPortSetup);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Termination mode strings.

void SerialSettings::setTxTermMode(const char* aTermMode)
{
   if (strcmp(aTermMode, "NULL") == 0) mTxTermMode = cSerialTermMode_Null;
   else if (strcmp(aTermMode, "LF") == 0) mTxTermMode = cSerialTermMode_LF;
   else if (strcmp(aTermMode, "CR") == 0) mTxTermMode = cSerialTermMode_CR;
   else if (strcmp(aTermMode, "CRLF") == 0) mTxTermMode = cSerialTermMode_CRLF;
   else mTxTermMode = cSerialTermMode_Null;
}

void SerialSettings::setRxTermMode(const char* aTermMode)
{
   if (strcmp(aTermMode, "NULL") == 0) mRxTermMode = cSerialTermMode_Null;
   else if (strcmp(aTermMode, "LF") == 0) mRxTermMode = cSerialTermMode_LF;
   else if (strcmp(aTermMode, "CR") == 0) mRxTermMode = cSerialTermMode_CR;
   else if (strcmp(aTermMode, "CRLF") == 0) mRxTermMode = cSerialTermMode_CRLF;
   else mRxTermMode = cSerialTermMode_Null;
}

char* SerialSettings::asStringTermMode(int aTermMode)
{
   switch (aTermMode)
   {
   case cSerialTermMode_Null: return "NULL";
   case cSerialTermMode_LF:   return "LF";
   case cSerialTermMode_CR:   return "CR";
   case cSerialTermMode_CRLF: return "CRLF";
   default: return "UNKNOWN";
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
#if 0
cSerialTermMode_Zero
cSerialTermMode_LF
cSerialTermMode_CR
cSerialTermMode_CRLF
#endif


}//namespace

