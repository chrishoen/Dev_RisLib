
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

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
   // Begin program.

   main_initialize(argc,argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   ProtoComm::gProcThread = new ProtoComm::ProcThread;
   ProtoComm::gProcThread->launchThread();

   ProtoComm::gMonitorThread = new ProtoComm::MonitorThread;
   ProtoComm::gMonitorThread->launchThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   ProtoComm::gProcThread->showThreadInfo();
   ProtoComm::gMonitorThread->showThreadInfo();

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

   ProtoComm::gMonitorThread->shutdownThread();
   delete ProtoComm::gMonitorThread;
   ProtoComm::gMonitorThread = 0;

   ProtoComm::gProcThread->shutdownThread();
   delete ProtoComm::gProcThread;
   ProtoComm::gProcThread = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
