#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "somePeriodicParms.h"
#include "someTimerTestThread1.h"
#include "someTimerTestThread2.h"
#include "someMonitorThread.h"

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

   if (Some::gPeriodicParms.mTestMode == 1)
   {
      Some::gTimerTestThread1 = new Some::TimerTestThread1;
      Some::gTimerTestThread1->launchThread();
   }
   else if (Some::gPeriodicParms.mTestMode == 2)
   {
      Some::gTimerTestThread2 = new Some::TimerTestThread2;
      Some::gTimerTestThread2->launchThread();
   }
   Some::gMonitorThread = new Some::MonitorThread;
   Some::gMonitorThread->launchThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (Some::gPeriodicParms.mTestMode == 1)
   {
      Some::gTimerTestThread1->showThreadInfo();
   }
   else if (Some::gPeriodicParms.mTestMode == 2)
   {
      Some::gTimerTestThread2->showThreadInfo();
   }
   Some::gMonitorThread->showThreadInfo();


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

   if (Some::gPeriodicParms.mTestMode == 1)
   {
      Some::gTimerTestThread1->shutdownThread();
      delete Some::gTimerTestThread1;
      Some::gTimerTestThread1 = 0;
   }
   else if (Some::gPeriodicParms.mTestMode == 2)
   {
      Some::gTimerTestThread2->shutdownThread();
      delete Some::gTimerTestThread2;
      Some::gTimerTestThread2 = 0;
   }
   Some::gMonitorThread->shutdownThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Finalize program.

   main_finalize();
   return 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
