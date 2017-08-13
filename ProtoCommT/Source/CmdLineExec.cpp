
#include "stdafx.h"

#include "procoSettings.h"
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
   if(aCmd->isCmd("SHUTDOWN"  ))  executeShutdown(aCmd);
   if(aCmd->isCmd("TX"        ))  executeTx(aCmd);
   if(aCmd->isCmd("PER"       ))  executePeriodic(aCmd);
   if(aCmd->isCmd("GO1"       ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"       ))  executeGo2(aCmd);
   if(aCmd->isCmd("T1"        ))  executeTest1(aCmd);
}

//******************************************************************************

void CmdLineExec::executeTx (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,201);

   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::Settings::cTcpServer:
         gServerThread->sendTestMsg(aCmd->argInt(1));
         break;
      case ProtoComm::Settings::cTcpClient:
         gClientThread->sendTestMsg();
         break;
   }
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);

   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
 
   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::Settings::cTcpServer:
         gServerThread->sendMsg(0,tMsg);
         break;
      case ProtoComm::Settings::cTcpClient:
         gClientThread->sendMsg(tMsg);
         break;
   }
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tN = aCmd->argInt(1);

   for (int i=0;i<tN;i++)
   {
      ProtoComm::StatusRequestMsg* tMsg = new ProtoComm::StatusRequestMsg;
 
      switch (ProtoComm::gSettings.mMyAppRole)
      {
         case ProtoComm::Settings::cTcpServer:
            gServerThread->sendMsg(0,tMsg);
            gServerThread->threadSleep(10);
            break;
         case ProtoComm::Settings::cTcpClient:
            gClientThread->sendMsg(tMsg);
            gClientThread->threadSleep(10);
            break;
      }
   }
}

//******************************************************************************

void CmdLineExec::executePeriodic(Ris::CmdLineCmd* aCmd)
{
   bool tPeriodicEnable=false;
   tPeriodicEnable = aCmd->argBool(1);

   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::Settings::cTcpServer:
         gServerThread->mPeriodicEnable = tPeriodicEnable;
         break;
      case ProtoComm::Settings::cTcpClient:
         gClientThread->mPeriodicEnable = tPeriodicEnable;
         break;
   }
}

//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
void CmdLineExec::executeShutdown (Ris::CmdLineCmd* aCmd)
{
   switch (ProtoComm::gSettings.mMyAppRole)
   {
      case ProtoComm::Settings::cTcpServer:
         gServerThread->shutdownThread();
         break;
      case ProtoComm::Settings::cTcpClient:
         gClientThread->shutdownThread();
         break;
   }
}
