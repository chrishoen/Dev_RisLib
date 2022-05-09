//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"
#include "risSerialSettings.h"

#define  _PROCOSERIALPARMS_CPP_
#include "procoSerialParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SerialParms::SerialParms()
{
   reset();
}

void SerialParms::reset()
{
   BaseClass::reset();
   if (Ris::portableIsWindows())
   {
      BaseClass::setFilePath("c:/aaa_prime/files/Serial_Parms.txt");
   }
   else
   {
      BaseClass::setFilePath_RelativeToBaseDir("files/Serial_Parms.txt");
   }

   mSerialPortDevice[0] = 0;
   mSerialPortSetup[0] = 0;
   mSerialRxTimeout = 0;
   mTxTermMode = 0;
   mRxTermMode = 0;
   mThreadTimerPeriod = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void SerialParms::show()
{
   printf("\n");
   printf("SerialParms************************************************ %s\n", mTargetSection);

   printf("SerialPortDevice        %-12s\n", mSerialPortDevice);
   printf("SerialPortSetup         %-12s\n", mSerialPortSetup);
   printf("SerialRxTimeout         %-12d\n", mSerialRxTimeout);

   printf("\n");
   printf("TxTermMode              %-12s\n", Ris::string_from_int_SerialSettingsTermMode(mTxTermMode));
   printf("RxTermMode              %-12s\n", Ris::string_from_int_SerialSettingsTermMode(mRxTermMode));

   printf("\n");
   printf("ThreadTimerPeriod       %-12d\n", mThreadTimerPeriod);

   printf("SerialParms************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void SerialParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("SerialPortDevice"))  aCmd->copyArgString(1, mSerialPortDevice, cMaxStringSize);
   if (aCmd->isCmd("SerialPortSetup"))   aCmd->copyArgString(1, mSerialPortSetup, cMaxStringSize);
   if (aCmd->isCmd("SerialRxTimeout"))   mSerialRxTimeout = aCmd->argInt(1);
   if (aCmd->isCmd("TxTermMode"))        mTxTermMode = Ris::int_from_string_SerialSettingsTermMode(aCmd->argString(1));
   if (aCmd->isCmd("RxTermMode"))        mRxTermMode = Ris::int_from_string_SerialSettingsTermMode(aCmd->argString(1));
   if (aCmd->isCmd("ThreadTimerPeriod")) mThreadTimerPeriod = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void SerialParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace