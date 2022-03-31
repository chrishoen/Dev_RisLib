#include "stdafx.h"


#include "risByteBuffer.h"
#include "risByteBlob.h"

#include "CmdLineExec.h"

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
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo7(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);
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

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1001);
   int tCount1 = aCmd->argInt(1);
   int tCount2 = 0;

   Ris::ByteBuffer tBuffer(1000);

   tBuffer.setCopyTo();
   tBuffer.copy ( &tCount1 );
   Prn::print(0,"Buffer.Length %d",tBuffer.mWorkingLength);

   tBuffer.rewind();
   tBuffer.setCopyFrom();

   tCount1=0;
   tBuffer.copy ( &tCount1 );
   Prn::print(0,"Count1        %d",tCount1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,false);
   bool tFlag1 = aCmd->argBool(1);

   Ris::ByteBuffer tBuffer(1000);

   tBuffer.setCopyTo();
   tBuffer.copy ( &tFlag1 );
   Prn::print(0,"Flag1          %5s",my_string_from_bool(tFlag1));
   Prn::print(0,"Buffer.Length  %5d",tBuffer.mWorkingLength);

   tBuffer.rewind();
   tBuffer.setCopyFrom();

   tFlag1 = false;
   tBuffer.copy ( &tFlag1 );
   Prn::print(0,"Flag1          %5s",my_string_from_bool(tFlag1));
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

