
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "procoSettings.h"
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
   if (aCmd->isCmd("SHUTDOWN" ))  executeOnShutdown (aCmd);
   if (aCmd->isCmd("TX"       ))  executeOnTx  (aCmd);
   if (aCmd->isCmd("GO1"      ))  executeOnGo1 (aCmd);
   if (aCmd->isCmd("GO2"      ))  executeOnGo2 (aCmd);
   if (aCmd->isCmd("GO3"      ))  executeOnGo3 (aCmd);
   if (aCmd->isCmd("GO4"      ))  executeOnGo4 (aCmd);
   if (aCmd->isCmd("GO5"      ))  executeOnGo5 (aCmd);
   if (aCmd->isCmd("GO6"      ))  executeOnGo6 (aCmd);
}
//******************************************************************************
void CmdLineExec::executeOnShutdown (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->shutdownThread();
}

//******************************************************************************

void CmdLineExec::executeOnTx (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   int tMsgType= aCmd->argInt(1);

   switch (tMsgType)
   {
      case 1:
      {
         ProtoComm::TestMsg* tMsg = new ProtoComm::TestMsg;
         tMsg->initialize();
         gNetworkThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
         tMsg->initialize();
         gNetworkThread->sendMsg(tMsg);
         break;
      }
   }
}

//******************************************************************************

void CmdLineExec::executeOnGo1 (Ris::CmdLineCmd* aCmd)
{
   gNetworkThread->sendTestMsg();
}

//******************************************************************************

void CmdLineExec::executeOnGo2(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::StatusRequestMsg* tMsg = new ProtoComm::StatusRequestMsg;

   gNetworkThread->sendMsg(tMsg);
}


//******************************************************************************

void CmdLineExec::executeOnGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;
   ProtoComm::DataMsg* tRxMsg = new ProtoComm::DataMsg;
   tTxMsg->initialize();

   ProtoComm::MsgCopier tCopier;

   tBuffer.setCopyTo();
   tCopier.copyToFrom(&tBuffer,tTxMsg);

   tBuffer.rewind();
   tBuffer.setCopyFrom();
   tCopier.copyToFrom(&tBuffer,tRxMsg);
   tRxMsg->show();

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo4(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;
   ProtoComm::DataMsg* tRxMsg = new ProtoComm::DataMsg;
   tTxMsg->initialize();

   tBuffer.putToBuffer((Ris::ByteContent*)tTxMsg);
   tBuffer.rewind();
// printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   Ris::BaseMsgMonkey* tMsgMonkey = new ProtoComm::MsgMonkey;

   tMsgMonkey->extractMessageHeaderParms(&tBuffer);

   // printf("Buffer2 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   // If the header is not valid then error
   if (!tMsgMonkey->mHeaderValidFlag)
   {
      Prn::print(0,"ERROR doRecv1 INVALID HEADER ");
      return;
   }

   tBuffer.rewind();
   tRxMsg = (ProtoComm::DataMsg*)tMsgMonkey->getMsgFromBuffer(&tBuffer);
   tRxMsg->show();

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo5(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataRecord* tTxMsg = new ProtoComm::DataRecord;
   ProtoComm::DataRecord* tRxMsg = new ProtoComm::DataRecord;
   tTxMsg->initialize();

   tBuffer.putToBuffer((Ris::ByteContent*)tTxMsg);
   printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.rewind();
   printf("Buffer2 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.getFromBuffer((Ris::ByteContent*)tRxMsg);
   printf("Buffer3 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tRxMsg->show();

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo6(Ris::CmdLineCmd* aCmd)
{
   ProtoComm::DataMsg* tMsg = new ProtoComm::DataMsg;
   tMsg->initialize();

   gNetworkThread->sendMsg(tMsg);
}


