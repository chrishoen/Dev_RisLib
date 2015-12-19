#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_functions.h"
#include "prnPrint.h"

#include "risByteBuffer.h"
#include "greenRecord.h"
#include "greenHelper.h"

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
   RecordCopier tCopier;

   TestRecord* tTxRecord = new TestRecord;
   TestRecord* tRxRecord = new TestRecord;
   Helper::initialize(tTxRecord);

   tBuffer.setCopyTo();
   tCopier.copyToFrom(&tBuffer,tTxRecord);
   printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   tBuffer.rewind();
   tBuffer.setCopyFrom();
   tCopier.copyToFrom(&tBuffer,tRxRecord);
   printf("Buffer1 %3d %3d %3d\n", tBuffer.getError(),tBuffer.getLength(),tBuffer.getPosition());

   Helper::show(tRxRecord);

   delete tTxRecord;
   delete tRxRecord;
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   Ris::ByteBuffer tBuffer(20000);
   RecordCopier tCopier;

   Data2Record* tTxRecord = new Data2Record;
   Data2Record* tRxRecord = new Data2Record;
   Helper::initialize(tTxRecord);

   tBuffer.setCopyTo();
   tCopier.copyToFrom(&tBuffer,tTxRecord);

   tBuffer.rewind();
   tBuffer.setCopyFrom();
   tCopier.copyToFrom(&tBuffer,tRxRecord);
   Helper::show(tRxRecord);

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

