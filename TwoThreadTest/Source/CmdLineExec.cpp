
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "CmdLineExec.h"

#include "someControllerThread.h"
using namespace Some;

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
   if(aCmd->isCmd("TP"    ))    executeTP        (aCmd);
   if(aCmd->isCmd("T1"    ))    executeTest1     (aCmd);
   if(aCmd->isCmd("T2"    ))    executeTest2     (aCmd);
   if(aCmd->isCmd("SEND"  ))    executeSend      (aCmd);
   if(aCmd->isCmd("RES"   ))    executeResponse  (aCmd);
   if(aCmd->isCmd("SEQ"   ))    executeSequence  (aCmd);
   if(aCmd->isCmd("X"     ))    executeAbort     (aCmd);
}

//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,true);
   gControllerThread->mTPFlag=aCmd->argBool(1);
}

//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10);
   gControllerThread->mTest1QCall(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeTest2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10);
   gControllerThread->mTest2QCall(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeSend(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,2);
   aCmd->setArgDefault(2,10);
   aCmd->setArgDefault(3,12);
   gControllerThread->mSendQCall(aCmd->argInt(1),aCmd->argInt(2),aCmd->argInt(3));
}

//******************************************************************************

void CmdLineExec::executeResponse(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,101);
   gControllerThread->mResponseQCall(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeSequence(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,109);
   aCmd->setArgDefault(2,100);
   aCmd->setArgDefault(3,10);
   aCmd->setArgDefault(4,2);

   int tId              = aCmd->argInt(1);
   int tIterations      = aCmd->argInt(2);
   int tCommandTimeout  = aCmd->argInt(3);
   int tResponseDelay   = aCmd->argInt(4);

   gControllerThread->mSequenceQCall(tId,tIterations,tCommandTimeout,tResponseDelay);
}

//******************************************************************************

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
   gControllerThread->abortWaitForNotify();
}


