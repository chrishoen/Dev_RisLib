
#include "stdafx.h"

#include "protoserialSettings.h"
#include "protoserialMsg.h"
#include "protoserialMsgHelper.h"

#include "protoserialSerialThread.h"

#include "CmdLineExec.h"

using namespace ProtoSerial;

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
   if (aCmd->isCmd("Parms"    ))  executeParms (aCmd);
}
//******************************************************************************
void CmdLineExec::executeOnShutdown (Ris::CmdLineCmd* aCmd)
{
   gSerialThread->shutdownThread();
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
         ProtoSerial::TestMsg* tMsg = new ProtoSerial::TestMsg;
         MsgHelper::initialize(tMsg);
         gSerialThread->sendMsg(tMsg);
         break;
      }
      case 5:
      {
         ProtoSerial::DataMsg* tMsg = new ProtoSerial::DataMsg;
         MsgHelper::initialize(tMsg);
         gSerialThread->sendMsg(tMsg);
         break;
      }
   }
}

//******************************************************************************

void CmdLineExec::executeOnGo1 (Ris::CmdLineCmd* aCmd)
{
   gSerialThread->sendTestMsg();
}

//******************************************************************************

void CmdLineExec::executeOnGo2(Ris::CmdLineCmd* aCmd)
{
   ProtoSerial::StatusRequestMsg* tMsg = new ProtoSerial::StatusRequestMsg;

   gSerialThread->sendMsg(tMsg);
}


//******************************************************************************

void CmdLineExec::executeOnGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoSerial::TestMsg* tTxMsg = new ProtoSerial::TestMsg;
   ProtoSerial::TestMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoSerial::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoSerial::TestMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo4(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoSerial::DataMsg* tTxMsg = new ProtoSerial::DataMsg;
   ProtoSerial::DataMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoSerial::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoSerial::DataMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************

void CmdLineExec::executeOnGo5(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoSerial::DataRecord* tTxMsg = new ProtoSerial::DataRecord;
   ProtoSerial::DataRecord* tRxMsg = new ProtoSerial::DataRecord;
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
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeOnGo6(Ris::CmdLineCmd* aCmd)
{
   ProtoSerial::DataMsg* tMsg = new ProtoSerial::DataMsg;
   MsgHelper::initialize(tMsg);

   gSerialThread->sendMsg(tMsg);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeParms(Ris::CmdLineCmd* aCmd)
{
   ProtoSerial::gSettings.show();
   return;

   aCmd->setArgDefault(1,"SerialPeer1");

   ProtoSerial::gSettings.reset();
   ProtoSerial::gSettings.readSection(aCmd->argString(1));
   ProtoSerial::gSettings.show();
}


