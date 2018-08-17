#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "someMasterThread.h"
#include "someSlaveThread.h"
using namespace Some;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize program

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch Threads

   gSlaveThread = new SlaveThread;
   gSlaveThread->launchThread();

   gMasterThread = new MasterThread;
   gMasterThread->launchThreads();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::executeCmdLineConsole(exec);
   delete exec;

   //--------------------------------------------------------------------
   // Shutdown Threads

   gMasterThread->shutdownThreads();
   delete gMasterThread;

   //gSlaveThread->shutdownThread();
   gSlaveThread->forceTerminateThread();
   delete gSlaveThread;

   //--------------------------------------------------------------------
   // Finalize program

   main_finalize();

   return 0;
}

