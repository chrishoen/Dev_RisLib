
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"
#include "MainArgs.h"

#include "procoServerThread.h"
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

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      ProtoComm::gServerThread = new ProtoComm::ServerThread;
      ProtoComm::gServerThread->launchThread();
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
   if (ProtoComm::gServerThread) ProtoComm::gServerThread->showThreadInfo();
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

   if (ProtoComm::gServerThread)
   {
      ProtoComm::gServerThread->shutdownThread();
      delete ProtoComm::gServerThread;
      ProtoComm::gServerThread = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
