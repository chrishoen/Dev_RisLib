#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <stdio.h>

#include "prnPrint.h"
#include "risCmdLineConsole.h"
#include "CmdLineExec.h"

void amain_init();


//******************************************************************************
int main(int argc,char** argv)
{
   //--------------------------------------------------------------------
   // Begin program

   amain_init();

   Prn::print(0,"amain***************************************************BEGIN");
   
   //--------------------------------------------------------------------
   // Start user command line executive,
   // Wait for user to exit

   CmdLineExec* exec = new CmdLineExec;
   Ris::gCmdLineConsole.execute(exec);
   delete exec;

   //--------------------------------------------------------------------
   // End program

   Prn::print(0,"amain*****************************************************END");
   return 0;
}
//******************************************************************************
void amain_init()
{
   // Initialize print facility
   Prn::initializePrint();

   Ris::setConsoleTitle("test");
}

