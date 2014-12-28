
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "procoMsg.h"

#include "procoNetworkThread.h"

#include "CmdLineExec.h"

using namespace ProtoComm;

//******************************************************************************
CmdLineExec::CmdLineExec()
{
   mRole=0;
}
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("SHUTDOWN"  ))  seqShutdown (aCmd);
   if(aCmd->isCmd("TX"        ))  seqTx       (aCmd);
   if(aCmd->isCmd("G1"        ))  seqGo1      (aCmd);
}
//******************************************************************************
void CmdLineExec::seqShutdown (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->shutdownThread();
}
//******************************************************************************

void CmdLineExec::seqTx (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestMsg();
}

//******************************************************************************

void CmdLineExec::seqGo1(Ris::CmdLineCmd* aCmd)
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


