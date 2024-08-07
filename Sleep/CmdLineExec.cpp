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
   double tStartTime;
   double tEndTime;
   double tDelta;
   Ris::RandomSleepMs tSleep(1000, 0.2);

   Prn::print(0, "BEGIN");
   tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   tEndTime = Ris::getProgramTime();
   tDelta = tEndTime - tStartTime;
   Prn::print(0, "END1 %.4f", tDelta);

   tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   tEndTime = Ris::getProgramTime();
   tDelta = tEndTime - tStartTime;
   Prn::print(0, "END2 %.4f", tDelta);

   tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   tEndTime = Ris::getProgramTime();
   tDelta = tEndTime - tStartTime;
   Prn::print(0, "END3 %.4f", tDelta);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   double tStartTime;
   double tEndTime;
   double tDelta;
   Ris::RandomSleepMs tSleep;
   tSleep.initialize(0, 1000);

   Prn::print(0, "BEGIN");
   tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   tEndTime = Ris::getProgramTime();
   tDelta = tEndTime - tStartTime;
   Prn::print(0, "END1 %.4f", tDelta);

   tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   tEndTime = Ris::getProgramTime();
   tDelta = tEndTime - tStartTime;
   Prn::print(0, "END2 %.4f", tDelta);

   tStartTime = Ris::getProgramTime();
   tSleep.doSleep();
   tEndTime = Ris::getProgramTime();
   tDelta = tEndTime - tStartTime;
   Prn::print(0, "END3 %.4f", tDelta);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

Ris::RandomSleepMs mSleep(1000, 0.2);

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
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

