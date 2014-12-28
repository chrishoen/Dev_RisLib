
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "procoMsg.h"

#include "procoClientThread.h"
#include "procoServerThread.h"

#include "risThreadsQueue.h"
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
   if(aCmd->isCmd("SHUTDOWN"  ))  seqShutdown(aCmd);
   if(aCmd->isCmd("TX"        ))  seqTx(aCmd);
   if(aCmd->isCmd("PER"       ))  seqPeriodic(aCmd);
   if(aCmd->isCmd("G1"        ))  seqGo1(aCmd);
   if(aCmd->isCmd("T1"        ))  seqTest1(aCmd);
}
//******************************************************************************
void CmdLineExec::seqShutdown (Ris::CmdLineCmd* aCmd)
{
   switch (mRole)
   {
   case SERVER_E:
      gServerThread->shutdownThread();
      break;
   case CLIENT_E:
      gClientThread->shutdownThread();
      break;
   }
}
//******************************************************************************

void CmdLineExec::seqTx (Ris::CmdLineCmd* aCmd)
{

   switch (mRole)
   {
   case SERVER_E:
      gServerThread->sendTestMsg();
      break;
   case CLIENT_E:
      gClientThread->sendTestMsg();
      break;
   }
}

//******************************************************************************

void CmdLineExec::seqPeriodic(Ris::CmdLineCmd* aCmd)
{
   bool tPeriodicEnable=false;
   tPeriodicEnable = aCmd->argBool(1);
   switch (mRole)
   {
   case SERVER_E:
      gServerThread->mPeriodicEnable = tPeriodicEnable;
      break;
   case CLIENT_E:
      gClientThread->mPeriodicEnable = tPeriodicEnable;
      break;
   }
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
   
      switch (mRole)
      {
      case SERVER_E:
         gServerThread->sendMsg(0,tTxMsg1);
       //Prn::print(0,0,"sendMsg %d",i);
         gServerThread->threadSleep(10);
         break;
      case CLIENT_E:
         gClientThread->sendMsg(tTxMsg1);
       //Prn::print(0,0,"sendMsg %d",i);
         gClientThread->threadSleep(10);
         break;
      }
   }
}

//******************************************************************************

void CmdLineExec::seqTest1(Ris::CmdLineCmd* aCmd)
{
   int tN;
   tN = aCmd->argInt(1);
   if(tN==0) tN=4;

   Ris::Threads::WaitableLimit tLimit(10);
   Prn::print(0,0,"BEGIN");
   for (int i=0;i<tN;i++)
   {
      Prn::print(0,0,"put %d",i);
      tLimit.put();
   }
   Prn::print(0,0,"END");
}

