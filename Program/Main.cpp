#include "stdafx.h"

#include "MainInit.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

int main(int argc,char** argv)
{
   printf("first line in main\n");
   //--------------------------------------------------------------------
   // Begin program

   main_initialize(argc,argv);

   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(exec);
   delete exec;

   //--------------------------------------------------------------------
   // End program

   printf("last line in main\n");
   return 0;
}

