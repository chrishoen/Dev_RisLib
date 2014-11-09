/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "risPortableCalls.h"
#include "prnPrint.h"

#define  _RISCMDLINECONSOLE_CPP_
#include "risCmdLineConsole.h"


namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
CmdLineConsole::CmdLineConsole()
{
}

CmdLineConsole::~CmdLineConsole()
{
}

void CmdLineConsole::execute(BaseCmdLineExec* aExec)
{            
   CmdLineCmd  tCmd;
   char tBuffer[200];

   aExec->reset();

   while(true)
   {
      // Read console input
      gets(tBuffer);

      // Test for toggle suppress print
      if (strcmp(tBuffer,"p")==0)
      {
         Prn::toggleSuppressPrint();
      }
      // Process buffer
      else
      {
         // Special case, nickname
         if (strcmp(tBuffer,"a")==0)
         {
            strcpy(tBuffer,"ABORT");
         }
         // Special case, nickname
         if (strcmp(tBuffer,"e")==0)
         {
            strcpy(tBuffer,"EXIT");
            return;
         }

         // Parse buffer to get command line command
         tCmd.parseCmdLine(tBuffer);

         // Test for valid command line command
         if (tCmd.mValidFlag)
         {
            // Test for exit
            if(tCmd.isCmd("EXIT"))
            {
               return;; 
            }
            // Not exit
            else
            {
               // Have executive execute the command line command
               aExec->execute(&tCmd);

               // Test for bad command
               // This is true if the executive didn't accept the command
               if(tCmd.isBadCmd()) printf("INVALID COMMAND\n");
            }
         }
         // Not valid command line command
         else
         {
            printf("INVALID COMMAND\n");
         }
      }
   }
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

void executeCmdLineConsole (BaseCmdLineExec* aExec)
{
   gCmdLineConsole.execute(aExec);

}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// RLR 05-79-36 combo
//******************************************************************************

void setConsoleTitle(char* aTitle)
{
   portableSetConsoleTitle(aTitle);
}

}//namespace

