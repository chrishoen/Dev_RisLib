#include <stdlib.h>
#include "prnPrint.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "exampleNetworkThread.h"

#include "MainInit.h"

using namespace Example;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Begin program

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch threads

   gNetworkThread = new NetworkThread;
   gNetworkThread->configure();
   gNetworkThread->launchThread();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(exec);
   delete exec;

   //--------------------------------------------------------------------
   // End program

   gNetworkThread->shutdownThread();
   delete gNetworkThread;

   main_finalize();

   return 0;
}
