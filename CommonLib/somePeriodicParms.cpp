//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "risCmdLineFile.h"

#define  _SOMEPERIODICPARMS_CPP_
#include "somePeriodicParms.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Some
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

PeriodicParms::PeriodicParms()
{
   reset();
}

void PeriodicParms::reset()
{
   BaseClass::reset();
   if (Ris::portableIsWindows())
   {
      BaseClass::setFilePath("c:/aaa_prime/files/Periodic_Parms.txt");
   }
   else
   {
      BaseClass::setFilePath("/opt/prime/files/Periodic_Parms.txt");
   }

   mMonitorThreadPeriod = 0;
   mTestThreadPeriod = 0;
   mSampleSize = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

void PeriodicParms::show()
{
   printf("\n");
   printf("PeriodicParms************************************************ %s\n", mTargetSection);

   printf("\n");
   printf("MonitorThreadPeriod      %-10d\n", mMonitorThreadPeriod);
   printf("TestThreadPeriod         %-10d\n", mTestThreadPeriod);
   printf("SampleSize               %-10d\n", mSampleSize);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Base class override: Execute a command from the command file to set a 
// member variable.  Only process commands for the target section.This is
// called by the associated command file object for each command in the file.

void PeriodicParms::execute(Ris::CmdLineCmd* aCmd)
{
   if (!isTargetSection(aCmd)) return;

   if (aCmd->isCmd("MonitorThreadPeriod"))   mMonitorThreadPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("TestThreadPeriod"))      mTestThreadPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("SampleSize"))            mSampleSize = aCmd->argInt(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Calculate expanded member variables. This is called after the entire
// section of the command file has been processed.

void PeriodicParms::expand()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace