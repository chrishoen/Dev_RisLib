
#include "stdafx.h"

#include "procoSerialParms.h"
#include "procoMsg.h"
#include "procoMsgHelper.h"

#include "procoProcThread.h"
#include "procoMonitorThread.h"

#include "CmdLineExec.h"

using namespace ProtoComm;

//******************************************************************************
//******************************************************************************
//******************************************************************************

CmdLineExec::CmdLineExec()
{
}
void CmdLineExec::reset()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if (aCmd->isCmd("TP"))        ProtoComm::gProcThread->mTPCode = aCmd->argInt(1);
   if (aCmd->isCmd("SEND"))      executeSend(aCmd);
   if (aCmd->isCmd("ECHO"))      executeEcho(aCmd);
   if (aCmd->isCmd("DATA"))      executeData(aCmd);
   if (aCmd->isCmd("ABORT"))     executeAbort(aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))       executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))       executeGo4(aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::special(int aSpecial)
{
   ProtoComm::gProcThread->mShowCode = aSpecial;
   ProtoComm::gMonitorThread->mShowCode = aSpecial;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSend (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tMsgType= aCmd->argInt(1);

   switch (tMsgType)
   {
      case 1:
      {
         ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
         MsgHelper::initialize(tMsg);
         gProcThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
         MsgHelper::initialize(tMsg);
         gProcThread->sendMsg(tMsg);
         break;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeEcho(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   int tNumWords = aCmd->argInt(1);
   
   ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;
   MsgHelper::initialize(tMsg,tNumWords);
   gProcThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeData(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   MsgHelper::initialize(tMsg);

   gProcThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort (Ris::CmdLineCmd* aCmd)
{
   gProcThread->mAbortQCall();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void test1(Ris::ByteContent* aMsg)
{
   ProtoComm::BaseMsg* tMsg = (ProtoComm::BaseMsg*)aMsg;
   Prn::print(0, "MessageType %d", tMsg->mMessageType);
}

void test2(Ris::ByteContent* aMsg)
{
   test1(aMsg);
}

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
   test1(tMsg);
   test2(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::MsgMetrics tMsgMetrics;
   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
   tMsgMetrics.update(tMsg, 100);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::gSerialParms.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

