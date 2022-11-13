#include "stdafx.h"


#include "risByteBuffer.h"
#include "risByteBlob.h"

#include "CmdLineExec.h"
// git test
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
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);

   if (aCmd->isCmd("GO6"))        executeGo6(aCmd);
   if (aCmd->isCmd("GO7"))        executeGo7(aCmd);
   if (aCmd->isCmd("GO8"))        executeGo8(aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   int tDataLen = aCmd->argInt(1);
   int tByteBoundary = 4;
  
   int tOffset = tDataLen % tByteBoundary;
   if (tOffset) tOffset = tByteBoundary - tOffset;
   int tNewDataLen = tDataLen + tOffset;
   Prn::print(0, "%d  %d", tDataLen, tNewDataLen);


#if 0
   int tNewDataLen =
      (tDataLen + sizeof(unsigned int) - 1) & ~(sizeof(unsigned int) - 1); //For safty reasons
   Prn::print(0, "%d  %d", tDataLen, tNewDataLen);
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,false);
   int tInt1 = 101;
   char tChar1 = 1;

   Ris::ByteBuffer tBuffer(1000);

   tBuffer.setCopyTo();
   tBuffer.copy(&tInt1);
   tBuffer.copy(&tChar1);
   Prn::print(0,"Buffer.Length  %5d",tBuffer.mWorkingLength);
   tBuffer.advanceToNextByteBoundary(4);
   Prn::print(0, "Buffer.Length  %5d", tBuffer.mWorkingLength);
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1, false);
   bool tFlag1 = aCmd->argBool(1);

   Ris::ByteBuffer tBuffer(1000);

   tBuffer.setCopyTo();
   tBuffer.copy(&tFlag1);
   Prn::print(0, "Flag1          %5s", my_string_from_bool(tFlag1));
   Prn::print(0, "Buffer.Length  %5d", tBuffer.mWorkingLength);

   tBuffer.rewind();
   tBuffer.setCopyFrom();

   tFlag1 = false;
   tBuffer.copy(&tFlag1);
   Prn::print(0, "Flag1          %5s", my_string_from_bool(tFlag1));
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo6(Ris::CmdLineCmd* aCmd)
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
   tOutput2.mCopySize = 8;
   tBuffer.copy(&tOutput2);

   printf("tOutput1 %8x\n", tOutput1);
   printf("tOutput2 ");
   for (int i = 0; i < tOutput2.mCopySize; i++) printf("%02x ", (int)tOutput2.mBytes[i]);
   printf("\n");

   tBuffer.show("tBuffer", 8);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   int tInput1 = 0x11111111;
   char tBytes1[8];
   char tBytes2[8];
   memset(tBytes1, 0x21, 8);
   memset(tBytes2, 0x22, 8);
   Ris::ByteBlob<1000> tInput2;
   tInput2.putBytes(tBytes1, 8);
   tInput2.putBytes(tBytes2, 8);
   printf("tInput2.mCopySize %d\n", tInput2.mCopySize);

   int tOutput1 = 0;
   Ris::ByteBlob<1000> tOutput2;

   tBuffer.setCopyTo();
   tBuffer.copy(&tInput1);
   tBuffer.copy(&tInput2);

   tBuffer.rewind();
   tBuffer.setCopyFrom();
   tBuffer.copy(&tOutput1);
   tOutput2.mCopySize = tInput2.mCopySize;
   tBuffer.copy(&tOutput2);

   printf("tOutput1 %8x\n", tOutput1);
   printf("tOutput2 ");
   for (int i = 0; i < tOutput2.mCopySize; i++) printf("%02x ", (int)tOutput2.mBytes[i]);
   printf("\n");

   tBuffer.show("tBuffer", 20);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo8(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);
}

