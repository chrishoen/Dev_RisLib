#include "prnPrint.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainInit.h"

#include "someControllerThread.h"
#include "somePlantThread.h"
using namespace Some;

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize program

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Launch Threads

   gPlantThread = new PlantThread;
   gPlantThread->launchThread();

   gControllerThread = new ControllerThread;
   gControllerThread->launchThreads();

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::executeCmdLineConsole(exec);
   delete exec;

   //--------------------------------------------------------------------
   // Shutdown Threads

   gControllerThread->shutdownThreads();
   delete gControllerThread;

   //gPlantThread->shutdownThread();
   gPlantThread->forceTerminateThread();
   delete gPlantThread;

   //--------------------------------------------------------------------
   // Finalize program

   main_finalize();

   return 0;
}

