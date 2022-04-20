#include "stdafx.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "string.h"

#include "risProgramTime.h"
#include "risBitUtils.h"
#include "risSleep.h"
#include "my_functions.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}

void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"))   reset();
   if (aCmd->isCmd("GO1"))    executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))    executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))    executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))    executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))    executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))    executeGo6(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "BEGIN");
   Ris::RandomSleepMs tSleep;
   tSleep.initializeMsUs(2000, 5000);
   double tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   double tEndTime = Ris::getProgramTime();
   double tDelta = tEndTime - tStartTime;
   Prn::print(0, "END %.4f", tDelta);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "BEGIN");
   Ris::RandomSleepMs tSleep;
   tSleep.initializeMsMs(5000, 200);
   double tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   double tEndTime = Ris::getProgramTime();
   double tDelta = tEndTime - tStartTime;
   Prn::print(0, "END %.4f", tDelta);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Prn::print(0, "BEGIN");
   Ris::RandomSleepMs tSleep;
   tSleep.initializeMsMs(2000, 0);
   double tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   double tEndTime = Ris::getProgramTime();
   double tDelta = tEndTime - tStartTime;
   Prn::print(0, "END %.4f", tDelta);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

