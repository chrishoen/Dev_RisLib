
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"
#include "MainArgs.h"

#include "procoProcThread.h"
#include "procoMonitorThread.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Initialize program resources.

   gMainArgs.initialize(argc, argv);
   gMainArgs.show();
   main_initialize();

   //main_test();
   //return 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      ProtoComm::gProcThread = new ProtoComm::ProcThread;
      ProtoComm::gProcThread->launchThread();
   }

   if (true)
   {
      ProtoComm::gMonitorThread = new ProtoComm::MonitorThread;
      ProtoComm::gMonitorThread->launchThread();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   if (ProtoComm::gProcThread) ProtoComm::gProcThread->showThreadInfo();
   if (ProtoComm::gMonitorThread) ProtoComm::gMonitorThread->showThreadInfo();

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

   if (ProtoComm::gMonitorThread)
   {
      ProtoComm::gMonitorThread->shutdownThread();
      delete ProtoComm::gMonitorThread;
      ProtoComm::gMonitorThread = 0;
   }

   if (ProtoComm::gProcThread)
   {
      ProtoComm::gProcThread->shutdownThread();
      delete ProtoComm::gProcThread;
      ProtoComm::gProcThread = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
