
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "procoSettings.h"
#include "procoDefs.h"
#include "procoMsg.h"

#include "procoClientThread.h"
#include "procoServerThread.h"

#include "risThreadsQueue.h"
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
   if(aCmd->isCmd("SHUTDOWN"  ))  executeOnShutdown(aCmd);
   if(aCmd->isCmd("TX"        ))  executeOnTx(aCmd);
   if(aCmd->isCmd("PER"       ))  executeOnPeriodic(aCmd);
   if(aCmd->isCmd("G1"        ))  executeOnGo1(aCmd);
   if(aCmd->isCmd("T1"        ))  executeOnTest1(aCmd);
}
//******************************************************************************
void CmdLineExec::executeOnShutdown (Ris::CmdLineCmd* aCmd)
{
   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::AR_TcpServer:
         gServerThread->shutdownThread();
         break;
      case ProtoComm::AR_TcpClient:
         gClientThread->shutdownThread();
         break;
   }
}
//******************************************************************************

void CmdLineExec::executeOnTx (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,201);

   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::AR_TcpServer:
         gServerThread->sendTestMsg(aCmd->argInt(1));
         break;
      case ProtoComm::AR_TcpClient:
         gClientThread->sendTestMsg();
         break;
   }
}

//******************************************************************************

void CmdLineExec::executeOnPeriodic(Ris::CmdLineCmd* aCmd)
{
   bool tPeriodicEnable=false;
   tPeriodicEnable = aCmd->argBool(1);

   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::AR_TcpServer:
         gServerThread->mPeriodicEnable = tPeriodicEnable;
         break;
      case ProtoComm::AR_TcpClient:
         gClientThread->mPeriodicEnable = tPeriodicEnable;
         break;
   }
}

//******************************************************************************

void CmdLineExec::executeOnGo1(Ris::CmdLineCmd* aCmd)
{
   int tN = aCmd->argInt(1);
   if(tN==0) tN=4;

   for (int i=0;i<tN;i++)
   {
      ProtoComm::StatusRequestMsg* tTxMsg1 = new ProtoComm::StatusRequestMsg;
   
      switch (ProtoComm::gSettings.mMyAppRole)
      {
         case ProtoComm::AR_TcpServer:
            gServerThread->sendMsg(0,tTxMsg1);
          //Prn::print(0,"sendMsg %d",i);
            gServerThread->threadSleep(10);
            break;
         case ProtoComm::AR_TcpClient:
            gClientThread->sendMsg(tTxMsg1);
          //Prn::print(0,"sendMsg %d",i);
            gClientThread->threadSleep(10);
            break;
      }
   }
}

//******************************************************************************

void CmdLineExec::executeOnTest1(Ris::CmdLineCmd* aCmd)
{
}

