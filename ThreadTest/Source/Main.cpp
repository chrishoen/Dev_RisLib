#include "prnPrint.h"
#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "someApcThread.h"
#include "someThread1.h"
#include "someApcThread.h"
#include "someQCallThread1.h"
#include "someTimerThread.h"
#include "Experiment.h"
#include "GSettings.h"
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

   if (gGSettings.mTestThread == GSettings::cTestThread_Thread1)
   {
      gThread1 = new Thread1;
      gThread1->launchThread();
   }

   if (gGSettings.mTestThread == GSettings::cTestThread_ApcThread)
   {
      gApcThread = new ApcThread;
      gApcThread->launchThread();
   }

   if (gGSettings.mTestThread == GSettings::cTestThread_QCallThread1)
   {
      gQCallThread1 = new QCallThread1;
      gQCallThread1->launchThread();
   }

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

   if (gGSettings.mTestThread == GSettings::cTestThread_Thread1)
   {
      gThread1->shutdownThread();
      delete gThread1;
   }

   if (gGSettings.mTestThread == GSettings::cTestThread_ApcThread)
   {
      gApcThread->shutdownThread();
      delete gApcThread;
   }

   if (gGSettings.mTestThread == GSettings::cTestThread_QCallThread1)
   {
      gQCallThread1->shutdownThread();
      delete gQCallThread1;
   }

   //--------------------------------------------------------------------
   // Exit
   
   main_finalize();

   return 0;
}

