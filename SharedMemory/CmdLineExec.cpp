#include "stdafx.h"

#include "risProgramTime.h"
#include "risThreadsThreads.h"
#include "risThreadsSynch.h"
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
   if (aCmd->isCmd("GO1"))  executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))  executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))  executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))  executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))  executeGo5(aCmd);

   if (aCmd->isCmd("SEMTEST"))  executeSemTest(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSemTest(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 10);
   Ris::Threads::NamedMutex tMutex("/mymutex");
   Prn::print(0, "mutext lock begin");
   tMutex.lock();
   Prn::print(0, "mutext lock end");

   for (int i = 0; i < aCmd->argInt(1); i++)
   {
      Prn::print(0, "sleep %d", i);
      Ris::Threads::threadSleep(1000);
   }

   Prn::print(0, "mutext unlock begin");
   tMutex.unlock();
   Prn::print(0, "mutext unlock end");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   double tTime = Ris::getCurrentProgramTime();
   Prn::print(0, "Ris::getCurrentProgramTime     %10.6f", tTime);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, "101");

   Prn::print(0, "test     %10d", aCmd->argInt(1));

   char tBuffer[40];
   Ris::portableGetCurrentTimeAsString(tBuffer, false);
   Prn::print(0, "Ris::portableGetCurrentTimeAsString   %s", tBuffer);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

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

