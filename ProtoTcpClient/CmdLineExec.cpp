
#include "stdafx.h"

#include "procoTcpSettings.h"
#include "procoMsg.h"
#include "procoMsgHelper.h"

#include "procoClientThread.h"

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
   if (aCmd->isCmd("TP"))        gClientThread->mTPFlag = aCmd->argBool(1);
   if (aCmd->isCmd("TX"))        executeTx(aCmd);
   if (aCmd->isCmd("ECHO"))      executeEcho(aCmd);
   if (aCmd->isCmd("DATA"))      executeData(aCmd);
   if (aCmd->isCmd("GO1"))       executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))       executeGo2(aCmd);
   if (aCmd->isCmd("T1"))        executeTest1(aCmd);
   if (aCmd->isCmd("Parms"))     executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTx (Ris::CmdLineCmd* aCmd)
{
   gClientThread->sendTestMsg();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeEcho(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 0);
   int tNumWords = aCmd->argInt(1);

   ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;
   MsgHelper::initialize(tMsg, tNumWords);
   gClientThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeData(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   MsgHelper::initialize(tMsg);

   gClientThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);

   ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;

   gClientThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tN = aCmd->argInt(1);

   for (int i=0;i<tN;i++)
   {
      ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;
 
      gClientThread->sendMsg(tMsg);
      gClientThread->threadSleep(10);
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::gTcpSettings.show();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
