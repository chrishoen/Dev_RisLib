#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someThread1.h"
#include "someTimerThread.h"
#include "Experiment.h"
#include "MainInit.h"

using namespace Some;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize

   main_initialize(argc,argv);
   Experiment::initialize();

   //--------------------------------------------------------------------
   // Launch threads

   gThread1 = new Thread1;
   gThread1->launchThread();

   gTimerThread = new TimerThread;
   gTimerThread->launchThread();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // It returns when user exits

   CmdLineExec* tExec = new CmdLineExec;
   Ris::executeCmdLineConsole(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // Shutdown threads

   gTimerThread->shutdownThread();
   delete gTimerThread;

   gThread1->shutdownThread();
   delete gThread1;

   //--------------------------------------------------------------------
   // Exit
   
   main_finalize();

   return 0;
}

