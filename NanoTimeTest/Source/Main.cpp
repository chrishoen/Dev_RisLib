#include "stdafx.h"

#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

#include "MainInit.h"

//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Initialize

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Start user command line executive,
   // It returns when user exits

   CmdLineExec* tExec = new CmdLineExec;
   Ris::executeCmdLineConsole(tExec);
   delete tExec;

   //--------------------------------------------------------------------
   // Exit
   
   main_finalize();

   return 0;
}

