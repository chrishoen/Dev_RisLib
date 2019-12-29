#include "stdafx.h"

#include "risProgramTime.h"
#include "risThreadsThreads.h"
#include "risThreadsSynch.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

//const char* gSemName = "Local\\mysem";
const char* gSemName = "mysem22";

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

   if (aCmd->isCmd("ST1"))  executeSemTest1(aCmd);
   if (aCmd->isCmd("ST2"))  executeSemTest2(aCmd);

   if (aCmd->isCmd("SemCreate"))  executeSemCreate(aCmd);
   if (aCmd->isCmd("SemOpen"))    executeSemOpen(aCmd);
   if (aCmd->isCmd("SemLock"))    executeSemLock(aCmd);
   if (aCmd->isCmd("SemUnlock"))  executeSemUnlock(aCmd);
   if (aCmd->isCmd("SemClose"))   executeSemClose(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSemTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 10);
   Ris::Threads::NamedMutex tMutex;
   if(!tMutex.create(gSemName)) return;

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

void CmdLineExec::executeSemTest2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 10);
   Ris::Threads::NamedMutex tMutex;
   if (!tMutex.open(gSemName)) return;

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

void CmdLineExec::executeSemCreate(Ris::CmdLineCmd* aCmd)
{
   gMutex.create(gSemName);
   Prn::print(0, "done");
}

void CmdLineExec::executeSemOpen(Ris::CmdLineCmd* aCmd)
{
   gMutex.open(gSemName);
   Prn::print(0, "done");
}

void CmdLineExec::executeSemLock(Ris::CmdLineCmd* aCmd)
{
   gMutex.lock();
   Prn::print(0, "done");
}

void CmdLineExec::executeSemUnlock(Ris::CmdLineCmd* aCmd)
{
   gMutex.unlock();
   Prn::print(0, "done");
}

void CmdLineExec::executeSemClose(Ris::CmdLineCmd* aCmd)
{
   gMutex.close();
   Prn::print(0, "done");
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

