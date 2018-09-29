#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "MainInit.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc, char** argv)
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Begin program.

   main_initialize(argc, argv);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Execute user command line executive, wait for user to exit.

   CmdLineExec* tExec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(tExec);
   delete tExec;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // End program.

   main_finalize();
   return 0;
}
