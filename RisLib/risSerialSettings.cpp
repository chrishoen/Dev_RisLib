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

SerialSettings::SerialSettings()
{
   reset();
}

void SerialSettings::reset()
{
   mPortDevice[0] = 0;
   mPortSetup[0] = 0;
   mRxTimeout = 0;
   mTermMode = 0;
   m485Flag = false;
   mPrintLevel = 0;
   mMonkeyCreator = 0;
   mThreadPriority = Ris::Threads::gPriorities.mSerial;
   mRxStringCallback = 0;
}

void SerialSettings::setPortDevice(char* aPortDevice)
{
   strcpy(mPortDevice, aPortDevice);
}
void SerialSettings::setPortSetup(char* aPortSetup)
{
   strcpy(mPortSetup, aPortSetup);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

