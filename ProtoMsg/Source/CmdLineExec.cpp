
#include "stdafx.h"

#include "procoMsg.h"
#include "procoMsgHelper.h"

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
   if (aCmd->isCmd("GO1"      ))  executeOnGo1 (aCmd);
   if (aCmd->isCmd("GO2"      ))  executeOnGo2 (aCmd);
   if (aCmd->isCmd("GO3"      ))  executeOnGo3 (aCmd);
   if (aCmd->isCmd("GO4"      ))  executeOnGo4 (aCmd);
   if (aCmd->isCmd("GO5"      ))  executeOnGo5 (aCmd);
   if (aCmd->isCmd("GO6"      ))  executeOnGo6 (aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeOnGo1 (Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;
   ProtoComm::DataMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer, tTxMsg);

   tRxMsg = (ProtoComm::DataMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeOnGo2(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeOnGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::TestMsg* tTxMsg = new ProtoComm::TestMsg;
   ProtoComm::TestMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoComm::TestMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeOnGo4(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;
   ProtoComm::DataMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoComm::DataMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeOnGo5(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataRecord* tTxMsg = new ProtoComm::DataRecord;
   ProtoComm::DataRecord* tRxMsg = new ProtoComm::DataRecord;
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
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

