#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "someTestThread.h"
#include "someMasterThread.h"
#include "someSlaveThread.h"
#include "someRandomTimerThread.h"

using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      Some::gTestChildThread = new Some::TestChildThread;
      Some::gTestChildThread->launchThread();
   }
   if (false)
   {
      Some::gMasterThread = new Some::MasterThread;
      Some::gMasterThread->launchThreads();
   }
   if (false)
   {
      Some::gSlaveThread = new Some::SlaveThread;
      Some::gSlaveThread->launchThread();
   }
   if (false)
   {
      Some::gRandomTimerThread1 = new Some::RandomTimerThread(1);
      Some::gRandomTimerThread1->launchThread();
   }
   if (false)
   {
      Some::gRandomTimerThread2 = new Some::RandomTimerThread(2);
      Some::gRandomTimerThread2->launchThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (Some::gTestChildThread) Some::gTestChildThread->showThreadInfo();
   if (Some::gSlaveThread) Some::gSlaveThread->showThreadInfo();
   if (Some::gMasterThread) Some::gMasterThread->showThreadInfo();
   if (Some::gRandomTimerThread1)Some::gRandomTimerThread1->showThreadInfo();
   if (Some::gRandomTimerThread2)Some::gRandomTimerThread2->showThreadInfo();
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Start user command line executive, wait for user to exit.

   CmdLineExec* exec = new CmdLineExec;
   Ris::executeCmdLineConsole(exec);
   delete exec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program Threads.

   if (Some::gTestChildThread)Some::gTestChildThread->shutdownThread();
   if (Some::gRandomTimerThread1) Some::gRandomTimerThread1->shutdownThread();
   if (Some::gRandomTimerThread2)Some::gRandomTimerThread2->shutdownThread();
   if (Some::gMasterThread)Some::gMasterThread->shutdownThreads();
   if (Some::gSlaveThread) Some::gSlaveThread->shutdownThread();

   if (Some::gTestChildThread) delete Some::gTestChildThread;
   if (Some::gRandomTimerThread1) delete Some::gRandomTimerThread1;
   if (Some::gRandomTimerThread2) delete Some::gRandomTimerThread2;
   if (Some::gMasterThread) delete Some::gMasterThread;
   if (Some::gSlaveThread) delete Some::gSlaveThread;

   if (Some::gTestChildThread) Some::gTestChildThread = 0;
   if (Some::gRandomTimerThread1) Some::gRandomTimerThread1 = 0;
   if (Some::gRandomTimerThread2) Some::gRandomTimerThread2 = 0;
   if (Some::gMasterThread) Some::gMasterThread = 0;
   if (Some::gSlaveThread) Some::gSlaveThread = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize program.

   main_finalize();
   return 0;

   printf("press enter\n");
   getchar();

   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
