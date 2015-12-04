
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "procoSettings.h"
#include "procoDefs.h"
#include "procoMsg.h"

#include "procoNetworkThread.h"

#include "CmdLineExec.h"

using namespace ProtoComm;

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
   if(aCmd->isCmd("SHUTDOWN"  ))  executeOnShutdown (aCmd);
   if (aCmd->isCmd("GO1"      ))  executeOnGo1(aCmd);
   if (aCmd->isCmd("GO2"      ))  executeOnGo2(aCmd);
   if (aCmd->isCmd("GO3"      ))  executeOnGo3(aCmd);
   if (aCmd->isCmd("GO4"      ))  executeOnGo4(aCmd);
}
//******************************************************************************
void CmdLineExec::executeOnShutdown (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->shutdownThread();
}
//******************************************************************************

void CmdLineExec::executeOnGo1 (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestMsg();
}

//******************************************************************************

void CmdLineExec::executeOnGo2(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::StatusRequestMsg* tTxMsg = new ProtoComm::StatusRequestMsg;

   gNetworkThread->sendMsg(tTxMsg);
}


//******************************************************************************

void CmdLineExec::executeOnGo3(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;

   gNetworkThread->sendMsg(tTxMsg);
}


//******************************************************************************

void CmdLineExec::executeOnGo4(Ris::CmdLineCmd* aCmd)
{
   int tN = aCmd->argInt(1);
   if (tN == 0) tN = 4;

   for (int i = 0; i<tN; i++)
   {
      ProtoComm::StatusRequestMsg* tTxMsg = new ProtoComm::StatusRequestMsg;

      gNetworkThread->sendMsg(tTxMsg);
      gNetworkThread->threadSleep(10);
   }
}
