#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_functions.h"
#include "prnPrint.h"

#include "risByteBuffer.h"
#include "greenRecord.h"

#include "CmdLineExec.h"

using namespace GreenRecord;

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
}

//******************************************************************************

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   TestRecord* tTxRecord = new TestRecord;
   TestRecord* tRxRecord = new TestRecord;
   tTxRecord->initialize();

   tBuffer.setCopyTo();
   RecordCopier::copyToFrom(&tBuffer,tTxRecord);
   printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.rewind();
   tBuffer.setCopyFrom();
   RecordCopier::copyToFrom(&tBuffer,tRxRecord);
   printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tRxRecord->show();

   delete tTxRecord;
   delete tRxRecord;
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);

   Data2Record* tTxRecord = new Data2Record;
   Data2Record* tRxRecord = new Data2Record;
   tTxRecord->initialize();

   tBuffer.setCopyTo();
   RecordCopier::copyToFrom(&tBuffer,tTxRecord);

   tBuffer.rewind();
   tBuffer.setCopyFrom();
   RecordCopier::copyToFrom(&tBuffer,tRxRecord);

   tRxRecord->show();

   delete tTxRecord;
   delete tRxRecord;
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
}

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

