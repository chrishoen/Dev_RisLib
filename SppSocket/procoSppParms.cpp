//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"
#include "risPortableCalls.h"
#include "risSerialSettings.h"

#define  _PROCOSPPPARMS_CPP_
#include "procoSppParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

SppParms::SppParms()
{
   reset();
}

void SppParms::reset()
{
   BaseClass::reset();
   BaseClass::setFilePath("c:/aaa_prime/files/Spp_Parms.txt");

   mRemoteName[0] = 0;
   mThreadTimerPeriod = 0;
   mTraceLogEnable = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void SppParms::show()
{
   printf("\n");
   printf("SppParms************************************************ %s\n", mTargetSection);

   printf("RemoteName              %-12s\n", mRemoteName);

   printf("\n");
   printf("ThreadTimerPeriod       %-12d\n", mThreadTimerPeriod);
   printf("TraceLogEnable          %-12s\n", my_string_from_bool(mTraceLogEnable));

   printf("SppParms************************************************\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void SppParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("RemoteName"))        aCmd->copyArgString(1, mRemoteName, cMaxStringSize);
   if (aCmd->isCmd("ThreadTimerPeriod")) mThreadTimerPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("TraceLogEnable"))    mTraceLogEnable = aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void SppParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace