
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "procoClientThread.h"
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

   main_initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Launch program threads.

   if (true)
   {
      ProtoComm::gClientThread = new ProtoComm::ClientThread;
      ProtoComm::gClientThread->launchThread();
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
   if (ProtoComm::gClientThread) ProtoComm::gClientThread->showThreadInfo();
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

   if (ProtoComm::gClientThread)
   {
      ProtoComm::gClientThread->shutdownThread();
      delete ProtoComm::gClientThread;
      ProtoComm::gClientThread = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
