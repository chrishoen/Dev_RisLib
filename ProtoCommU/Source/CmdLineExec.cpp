
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
   if(aCmd->isCmd("TX"        ))  executeOnTx       (aCmd);
   if(aCmd->isCmd("G1"        ))  executeOnGo1      (aCmd);
}
//******************************************************************************
void CmdLineExec::executeOnShutdown (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->shutdownThread();
}
//******************************************************************************

void CmdLineExec::executeOnTx (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestMsg();
}

//******************************************************************************

void CmdLineExec::executeOnGo1(Ris::CmdLineCmd* aCmd)
{
   int tN = aCmd->argInt(1);
   if(tN==0) tN=4;

   for (int i=0;i<tN;i++)
   {
      ProtoComm::StatusRequestMsg* tTxMsg1 = new ProtoComm::StatusRequestMsg;
   
      Ris::TimeMarker  tTimeMarker;
      tTimeMarker.doBegin();
      tTxMsg1->mTimeMarker=tTimeMarker;
   
      gNetworkThread->sendMsg(tTxMsg1);
      gNetworkThread->threadSleep(10);
   }
}


