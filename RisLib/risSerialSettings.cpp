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
// Helpers. Serial port settings termination mode strings.

int int_from_string_SerialSettingsTermMode(const char* aTermMode)
{
   if (strcmp(aTermMode, "NULL") == 0) return cSerialTermMode_Null;
   else if (strcmp(aTermMode, "LF") == 0) return cSerialTermMode_LF;
   else if (strcmp(aTermMode, "CR") == 0) return cSerialTermMode_CR;
   else if (strcmp(aTermMode, "CRLF") == 0) return cSerialTermMode_CRLF;
   return cSerialTermMode_Null;
}

char* string_from_int_SerialSettingsTermMode(int aTermMode)
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

