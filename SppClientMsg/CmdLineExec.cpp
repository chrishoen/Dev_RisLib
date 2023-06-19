#include "stdafx.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "string.h"

#include "risBtSockets.h"
#include "risBtFindAddress.h"
#include "procoSppParms.h"

#include "procoMsg.h"
#include "procoMsgHelper.h"

#include "procoClientThread.h"
#include "procoMonitorThread.h"


#include "CmdLineExec.h"

using namespace ProtoComm;

//******************************************************************************
//******************************************************************************
//******************************************************************************

Ris::BtSockets::BtSocketAddress mRemoteAddress;
Ris::BtSockets::BaseSppStreamSocket mRemoteSocket;

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
// This class is the program command line executive. It processes user
// command line inputs and executes them. It inherits from the command line
// command executive base class, which provides an interface for executing
// command line commands. It provides an override execute function that is
// called by a console executive when it receives a console command line input.
// The execute function then executes the command.

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"))   reset();

   if (aCmd->isCmd("TP"))        ProtoComm::gClientThread->mTPCode = aCmd->argInt(1);
   if (aCmd->isCmd("SEND"))      executeSend(aCmd);
   if (aCmd->isCmd("ECHO"))      executeEcho(aCmd);
   if (aCmd->isCmd("DATA"))      executeData(aCmd);
   if (aCmd->isCmd("ABORT"))     executeAbort(aCmd);

   if (aCmd->isCmd("FIND"))   executeFind(aCmd);
   if (aCmd->isCmd("CONN"))   executeConnect(aCmd);
   if (aCmd->isCmd("CL"))     executeClose(aCmd);

   if (aCmd->isCmd("GO1"))    executeGo1(aCmd);
   if (aCmd->isCmd("GO2"))    executeGo2(aCmd);
   if (aCmd->isCmd("GO3"))    executeGo3(aCmd);
   if (aCmd->isCmd("GO4"))    executeGo4(aCmd);
   if (aCmd->isCmd("GO5"))    executeGo5(aCmd);
   if (aCmd->isCmd("GO6"))    executeGo6(aCmd);

   if (aCmd->isCmd("PARMS"))  executeParms(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::special(int aSpecial)
{
   ProtoComm::gClientThread->mShowCode = aSpecial;
   ProtoComm::gMonitorThread->mShowCode = aSpecial;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeSend(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, 1);
   int tMsgType = aCmd->argInt(1);

   switch (tMsgType)
   {
   case 1:
   {
      ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
      MsgHelper::initialize(tMsg);
      gClientThread->sendMsg(tMsg);
      break;
   }
   case 5:
   {
      ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
      MsgHelper::initialize(tMsg);
      gClientThread->sendMsg(tMsg);
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

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeFind(Ris::CmdLineCmd* aCmd)
{
   if (!Ris::BtSockets::doFindAddressFromName(
      &mRemoteAddress,
      ProtoComm::gSppParms.mRemoteName,
      aCmd->argBool(1)))
   {
      Prn::print(0, "FAIL");
      return;
   }

   Prn::print(0, "address %012llx", mRemoteAddress.mBtAddress.btAddr);
   Prn::print(0, "port    %d", mRemoteAddress.mBtAddress.port);

   mRemoteSocket.mRemote.reset();
   mRemoteSocket.mRemote = mRemoteAddress;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeConnect(Ris::CmdLineCmd* aCmd)
{
   mRemoteSocket.doSocket();
   if (!mRemoteSocket.doConnect())
   {
      Prn::print(0, "FAIL");
      return;
   }
   Prn::print(0, "Pass");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeClose(Ris::CmdLineCmd* aCmd)
{
   mRemoteSocket.doClose();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
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

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::gSppParms.show();
}

