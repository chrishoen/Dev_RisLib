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
      BaseClass::setFilePath_RelativeToBaseDir("files/Periodic_Parms.txt");
   }

   mTestMode = 1;
   mTestThreadProcessor = -1;
   mTestThreadPriority = 80;
   mPollProcessor = false;

   mMonitorThreadPeriod = 0;
   mStatPeriod = 0;
   mTestThreadPeriodUs = 0;

   mRandomThreadPeriodUs1 = 0;
   mRandomThreadPeriodUs2 = 0;

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
   printf("TestMode                 %-10d\n", mTestMode);
   printf("\n");
   printf("TestThreadProcessor      %-10d\n", mTestThreadProcessor);
   printf("TestThreadPriority       %-10d\n", mTestThreadPriority);
   printf("PollProcessor            %-10s\n", my_string_from_bool(mPollProcessor));
   printf("\n");
   printf("MonitorThreadPeriod      %-10d\n", mMonitorThreadPeriod);
   printf("StatPeriod               %-10d\n", mStatPeriod);
   printf("TestThreadPeriodUs       %-10d\n", mTestThreadPeriodUs);
   printf("\n");
   printf("RandomThreadPeriodUs     %-10d  %-10d\n", mRandomThreadPeriodUs1, mRandomThreadPeriodUs2);
   printf("\n");
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

   if (aCmd->isCmd("TestMode"))              mTestMode = aCmd->argInt(1);

   if (aCmd->isCmd("TestThreadProcessor"))   mTestThreadProcessor = aCmd->argInt(1);
   if (aCmd->isCmd("TestThreadPriority"))    mTestThreadPriority = aCmd->argInt(1);
   if (aCmd->isCmd("PollProcessor"))         mPollProcessor = aCmd->argBool(1);

   if (aCmd->isCmd("MonitorThreadPeriod"))   mMonitorThreadPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("StatPeriod"))            mStatPeriod = aCmd->argInt(1);
   if (aCmd->isCmd("TestThreadPeriodUs"))    mTestThreadPeriodUs = aCmd->argInt(1);

   if (aCmd->isCmd("RandomThreadPeriodUs"))
   {
      mRandomThreadPeriodUs1 = aCmd->argInt(1);
      mRandomThreadPeriodUs2 = aCmd->argInt(2);
   }
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