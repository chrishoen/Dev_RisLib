#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someThread1.h"
#include "someTimerThread.h"
#include "Experiment.h"
#include "MainInit.h"

using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc, char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program resources.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      Some::gThread1 = new Some::Thread1;
      Some::gThread1->launchThread();
   }

   if (true)
   {
      Some::gTimerThread = new Some::TimerThread;
      Some::gTimerThread->launchThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (Some::gThread1) Some::gThread1->showThreadInfo();
   if (Some::gTimerThread) Some::gTimerThread->showThreadInfo();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute console command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Shutdown program threads.

   if (Some::gThread1)
   {
      Some::gThread1->shutdownThread();
      delete Some::gThread1;
      Some::gThread1 = 0;
   }

   if (Some::gTimerThread)
   {
      Some::gTimerThread->shutdownThread();
      delete Some::gTimerThread;
      Some::gTimerThread = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}

