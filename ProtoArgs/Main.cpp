
#include "stdafx.h"

#include "risThreadsProcess.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"
#include "MainArgs.h"
#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   gMainArgs.initialize(argc, argv);
   main_initialize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   // If the command line interface is enabled.
   if (gMainArgs.mCLIEnable)
   {
      // Execute console command line executive, wait for user to exit.
      CmdLineExec* tExec = new CmdLineExec;
      Ris::gCmdLineConsole.execute(tExec);
      delete tExec;
   }
   else
   {
      // Wait for program termination. 
      Ris::Threads::waitForTermination();
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
