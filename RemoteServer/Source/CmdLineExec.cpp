
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "my_functions.h"
#include "risRemoteServerThread.h"
#include "risPortableCalls.h"

#include "CmdLineExec.h"

//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("T1"    ))  executeTest1   (aCmd);
   if(aCmd->isCmd("CL"    ))  executeCmdLine (aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10);
   Prn::print(0,0,"Test1");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeCmdLine(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10);

   // Command line string
   char tCmdLineStr[200];

   bool tGoing=true;
   while(tGoing)
   {
      // Wait for key pressed
//    Ris::portableWaitForKbhit();

      // Get command line string
      fgets(tCmdLineStr,200,stdin);
      my_trimCRLF(tCmdLineStr);
      Ris::portableStrupr(tCmdLineStr);

      // If string not empty
      if (strlen(tCmdLineStr)!=0)
      {
         // Test for ESC
         if(strcmp(tCmdLineStr,"ESC")==0)
         {
            tGoing=false; 
         }
         // Not ESC
         else
         {
            // Send message to remote client with command line string
            Ris::RemoteMsg::CmdLineStrMsg* tTxMsg = new Ris::RemoteMsg::CmdLineStrMsg(tCmdLineStr);
            Ris::Remote::gServerThread->sendMsgToRemote(tTxMsg);
         }
      }
   }
}

