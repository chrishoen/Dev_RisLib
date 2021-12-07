//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"


#define  _SOMESERIALPARMS_CPP_
#include "someSerialParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
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
   BaseClass::setFilePath("/opt/prime/files/Serial_Parms.txt");

   mMyAppNumber = 0;
   mSerialPortDevice[0] = 0;
   mSerialPortSetup[0] = 0;
   mSerialRxTimeout = 0;
   mThreadTimerPeriod = 0;
   mNumWords = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void SerialParms::show()
{
   printf("\n");
   printf("SerialParms************************************************ %s\n", mTargetSection);

   printf("MyAppNumber             %16d\n", mMyAppNumber);

   printf("SerialPortDevice           %-12s\n", mSerialPortDevice);
   printf("SerialPortSetup            %-12s\n", mSerialPortSetup);
   printf("SerialRxTimeout            %5d\n", mSerialRxTimeout);

   printf("\n");
   printf("ThreadTimerPeriod       %16d\n", mThreadTimerPeriod);
   printf("NumWords                %16d\n", mNumWords);

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

   if (aCmd->isCmd("MyAppNumber"))         mMyAppNumber = aCmd->argInt(1);

   if (aCmd->isCmd("SerialPortDevice"))  aCmd->copyArgString(1, mSerialPortDevice, cMaxStringSize);
   if (aCmd->isCmd("SerialPortSetup"))   aCmd->copyArgString(1, mSerialPortSetup, cMaxStringSize);
   if (aCmd->isCmd("SerialRxTimeout"))   mSerialRxTimeout = aCmd->argInt(1);

   if (aCmd->isCmd("ThreadTimerPeriod"))   mThreadTimerPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("NumWords"))            mNumWords = aCmd->argInt(1);
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