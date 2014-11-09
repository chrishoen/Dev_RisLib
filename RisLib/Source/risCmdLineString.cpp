/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>

#include "risCmdLineString.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
CmdLineString::CmdLineString()
{
}

char* CmdLineString::execute (BaseCmdLineExec* aExec, char* aCmdLineString)
{
   // Command
   CmdLineCmd  tCmd;

   // Parse command line string
   tCmd.parseCmdLine(aCmdLineString);

   // Test for exit command
   if(tCmd.isCmd("EXIT"))
   {
      return 0;
   }

   // Execute the executive with the command
   aExec->execute(&tCmd);

   // Test for bad command
   if(tCmd.isBadCmd())
   {
      printf("executeCmdLineString INVALID COMMAND\n");
   }

   // return command response

   if (tCmd.getResponse()!=0)
   {
      strcpy(mResponseString,tCmd.getResponse());
      return &mResponseString[0];
   }
   else
   {
      return 0;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace

