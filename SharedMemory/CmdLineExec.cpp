#include "stdafx.h"

#include "risProgramTime.h"
#include "risThreadsThreads.h"
#include "risThreadsSynch.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

//const char* gMutexName = "Local\\mysem";
const char* gMutexName = "mymutex";
const char* mSemaphoreName = "mysemaphore";

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

   if (aCmd->isCmd("ST1"))  executeMutexTest1(aCmd);
   if (aCmd->isCmd("ST2"))  executeMutexTest2(aCmd);

   if (aCmd->isCmd("MutexCreate"))  executeMutexCreate(aCmd);
   if (aCmd->isCmd("MutexOpen"))    executeMutexOpen(aCmd);
   if (aCmd->isCmd("MutexLock"))    executeMutexLock(aCmd);
   if (aCmd->isCmd("MutexUnlock"))  executeMutexUnlock(aCmd);
   if (aCmd->isCmd("MutexClose"))   executeMutexClose(aCmd);

   if (aCmd->isCmd("SemCreate"))  executeSemaphoreCreate(aCmd);
   if (aCmd->isCmd("SemOpen"))    executeSemaphoreOpen(aCmd);
   if (aCmd->isCmd("SemGet"))     executeSemaphoreGet(aCmd);
   if (aCmd->isCmd("SemPut"))     executeSemaphorePut(aCmd);
   if (aCmd->isCmd("SemClose"))   executeSemaphoreClose(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeMutexTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 10);
   Ris::Threads::NamedMutex tMutex;
   if(!tMutex.create(gMutexName)) return;

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

void CmdLineExec::executeMutexTest2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 10);
   Ris::Threads::NamedMutex tMutex;
   if (!tMutex.open(gMutexName)) return;

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

void CmdLineExec::executeMutexCreate(Ris::CmdLineCmd* aCmd)
{
   mMutex.create(gMutexName);
   Prn::print(0, "done");
}

void CmdLineExec::executeMutexOpen(Ris::CmdLineCmd* aCmd)
{
   mMutex.open(gMutexName);
   Prn::print(0, "done");
}

void CmdLineExec::executeMutexLock(Ris::CmdLineCmd* aCmd)
{
   mMutex.lock();
   Prn::print(0, "done");
}

void CmdLineExec::executeMutexUnlock(Ris::CmdLineCmd* aCmd)
{
   mMutex.unlock();
   Prn::print(0, "done");
}

void CmdLineExec::executeMutexClose(Ris::CmdLineCmd* aCmd)
{
   mMutex.close();
   Prn::print(0, "done");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSemaphoreCreate(Ris::CmdLineCmd* aCmd)
{
   mSemaphore.create(mSemaphoreName);
   Prn::print(0, "done");
}

void CmdLineExec::executeSemaphoreOpen(Ris::CmdLineCmd* aCmd)
{
   mSemaphore.open(mSemaphoreName);
   Prn::print(0, "done");
}

void CmdLineExec::executeSemaphoreGet(Ris::CmdLineCmd* aCmd)
{
   mSemaphore.get();
   Prn::print(0, "done");
}

void CmdLineExec::executeSemaphorePut(Ris::CmdLineCmd* aCmd)
{
   mSemaphore.put();
   Prn::print(0, "done");
}

void CmdLineExec::executeSemaphoreClose(Ris::CmdLineCmd* aCmd)
{
   mSemaphore.close();
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

