
#include "stdafx.h"

#include "procoSerialParms.h"
#include "procoMsg.h"
#include "procoMsgHelper.h"

#include "procoSerialThread.h"

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
   if (aCmd->isCmd("TP"))        ProtoComm::gSerialThread->mTPCode = aCmd->argInt(1);
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
   if (aSpecial == 0)
   {
      ProtoComm::gSerialThread->mTPCode = 0;
      ProtoComm::gSerialThread->mShowCode = 0;
   }
   else if (aSpecial == 1)
   {
      ProtoComm::gSerialThread->mTPCode = 1;
      ProtoComm::gSerialThread->mShowCode = 1;
   }
   else if (aSpecial == 2)
   {
      ProtoComm::gSerialThread->mTPCode = 2;
      ProtoComm::gSerialThread->mShowCode = 2;
   }
   else if (aSpecial == 3)
   {
      ProtoComm::gSerialThread->mTPCode = 3;
      ProtoComm::gSerialThread->mShowCode = 3;
   }
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
         gSerialThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
         MsgHelper::initialize(tMsg);
         gSerialThread->sendMsg(tMsg);
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
   gSerialThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeData(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   MsgHelper::initialize(tMsg);

   gSerialThread->sendMsg(tMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeAbort (Ris::CmdLineCmd* aCmd)
{
   gSerialThread->mAbortQCall();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   gSerialThread->sendTestMsg();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::EchoRequestMsg* tMsg = new ProtoComm::EchoRequestMsg;

   gSerialThread->sendMsg(tMsg);
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

