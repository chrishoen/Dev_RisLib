
#include "stdafx.h"

#include "exampleSettings.h"
#include "exampleMsg.h"
#include "exampleMsgHelper.h"

#include "exampleNetworkThread.h"

#include "CmdLineExec.h"

using namespace ExampleMsg;

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
         ExampleMsg::TestMsg* tMsg = new ExampleMsg::TestMsg;
         MsgHelper::initialize(tMsg);
         gNetworkThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ExampleMsg::DataMsg* tMsg = new ExampleMsg::DataMsg;
         MsgHelper::initialize(tMsg);
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
   ExampleMsg::StatusRequestMsg* tMsg = new ExampleMsg::StatusRequestMsg;

   gNetworkThread->sendMsg(tMsg);
}


//******************************************************************************

void CmdLineExec::executeOnGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ExampleMsg::TestMsg* tTxMsg = new ExampleMsg::TestMsg;
   ExampleMsg::TestMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ExampleMsg::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ExampleMsg::TestMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo4(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ExampleMsg::DataMsg* tTxMsg = new ExampleMsg::DataMsg;
   ExampleMsg::DataMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ExampleMsg::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ExampleMsg::DataMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo5(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ExampleMsg::DataRecord* tTxMsg = new ExampleMsg::DataRecord;
   ExampleMsg::DataRecord* tRxMsg = new ExampleMsg::DataRecord;
   MsgHelper::initialize(tTxMsg);

   tBuffer.putToBuffer((Ris::ByteContent*)tTxMsg);
   printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.rewind();
   printf("Buffer2 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.getFromBuffer((Ris::ByteContent*)tRxMsg);
   printf("Buffer3 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo6(Ris::CmdLineCmd* aCmd)
{
   ExampleMsg::DataMsg* tMsg = new ExampleMsg::DataMsg;
   MsgHelper::initialize(tMsg);

   gNetworkThread->sendMsg(tMsg);
}


