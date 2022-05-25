
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "procoPeerThread.h"
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

   ProtoComm::gPeerThread = new ProtoComm::PeerThread;
   ProtoComm::gPeerThread->launchThread();

   ProtoComm::gMonitorThread = new ProtoComm::MonitorThread;
   ProtoComm::gMonitorThread->launchThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Show program threads.

   Ris::Threads::showCurrentThreadInfo();
   ProtoComm::gPeerThread->showThreadInfo();
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

   ProtoComm::gPeerThread->shutdownThread();
   delete ProtoComm::gPeerThread;
   ProtoComm::gPeerThread = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
