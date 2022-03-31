
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
   if (aCmd->isCmd("GO1"      ))  executeGo1 (aCmd);
   if (aCmd->isCmd("GO2"      ))  executeGo2 (aCmd);
   if (aCmd->isCmd("GO3"      ))  executeGo3 (aCmd);
   if (aCmd->isCmd("GO4"      ))  executeGo4 (aCmd);
   if (aCmd->isCmd("GO5"      ))  executeGo5 (aCmd);
   if (aCmd->isCmd("GO6"      ))  executeGo6 (aCmd);

   if (aCmd->isCmd("GO7"))        executeGo7(aCmd);
   if (aCmd->isCmd("GO8"))        executeGo8(aCmd);
   if (aCmd->isCmd("GO9"))        executeGo9(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   int tInput1 = 0x11111111;
   Ris::ByteBlob<1000> tInput2;
   tInput2.mCopySize = 8;
   for (int i = 0; i < 8; i++) tInput2.mBytes[i] = 0x22;
   int tOutput1 = 0;
   Ris::ByteBlob<1000> tOutput2;

   tBuffer.setCopyTo();
   tBuffer.copy(&tInput1);
   tBuffer.copy(&tInput2);

   tBuffer.rewind();
   tBuffer.setCopyFrom();
   tBuffer.copy(&tOutput1);
   tBuffer.copy(&tOutput2);

   printf("tOutput1 %8x\n", tOutput1);
   printf("tOutput2 ");
   for (int i = 0; i < tOutput2.mCopySize; i++) printf("%02x ", (int)tOutput2.mBytes[i]);
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo8(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo9(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::ByteBlobMsg* tTxMsg = new ProtoComm::ByteBlobMsg;
   ProtoComm::ByteBlobMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer, tTxMsg);

   tRxMsg = (ProtoComm::ByteBlobMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(0, tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;
   ProtoComm::DataMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer, tTxMsg);

   tRxMsg = (ProtoComm::DataMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(0, tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::TestMsg* tTxMsg = new ProtoComm::TestMsg;
   ProtoComm::TestMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoComm::TestMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(0, tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   ProtoComm::DataMsg* tTxMsg = new ProtoComm::DataMsg;
   ProtoComm::DataMsg* tRxMsg = 0;
   MsgHelper::initialize(tTxMsg);

   ProtoComm::MsgMonkey tMonkey;

   tMonkey.putMsgToBuffer(&tBuffer,tTxMsg);

   tRxMsg = (ProtoComm::DataMsg*)tMonkey.makeMsgFromBuffer(&tBuffer);

   MsgHelper::show(0, tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
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

   MsgHelper::show(0, tRxMsg);

   delete tTxMsg;
   delete tRxMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
{
   char tString[100];
   strcpy(tString, "abcdefgh");
   printf("strlen %d\n", (int)strlen(tString));

}

//******************************************************************************
//******************************************************************************
//******************************************************************************

