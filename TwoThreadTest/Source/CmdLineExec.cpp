#include "stdafx.h"


#include "CmdLineExec.h"

#include "someMasterThread.h"
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
   if(aCmd->isCmd("TP"    ))    executeTP          (aCmd);
   if(aCmd->isCmd("T1"    ))    executeTest1       (aCmd);
   if(aCmd->isCmd("T2"    ))    executeTest2       (aCmd);
   if(aCmd->isCmd("SEND"  ))    executeSend        (aCmd);
   if(aCmd->isCmd("RES"   ))    executeResponse    (aCmd);
   if(aCmd->isCmd("S1"    ))    executeSequence1   (aCmd);
   if(aCmd->isCmd("S2"    ))    executeSequence2   (aCmd);
   if(aCmd->isCmd("X"     ))    executeAbort       (aCmd);
   if(aCmd->isCmd("WORK"  ))    executeWorkRequest (aCmd);
}

//******************************************************************************

void CmdLineExec::executeWorkRequest(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,0);
   aCmd->setArgDefault(2,0);
   gMasterThread->mSendWorkRequestQCall(aCmd->argInt(1),aCmd->argInt(2));
}

//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,true);
   gMasterThread->mTPFlag=aCmd->argBool(1);
}

//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10000);
   gMasterThread->mTest1QCall(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeTest2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10000);
   gMasterThread->mTest2QCall(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeSend(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   aCmd->setArgDefault(2,2);
   aCmd->setArgDefault(3,12);
   gMasterThread->mSendCommandQCall(aCmd->argInt(1),aCmd->argInt(2),aCmd->argInt(3));
}

//******************************************************************************

void CmdLineExec::executeResponse(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,101);
   gMasterThread->mResponseQCall(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeSequence1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1);
   aCmd->setArgDefault(2,1);
   aCmd->setArgDefault(3,10);
   aCmd->setArgDefault(4,1);

   int tId              = aCmd->argInt(1);
   int tIterations      = aCmd->argInt(2);
   int tCommandTimeout  = aCmd->argInt(3);
   int tResponseDelay   = aCmd->argInt(4);

   gMasterThread->mSequenceQCall(tId,tIterations,tCommandTimeout,tResponseDelay);
}

//******************************************************************************

void CmdLineExec::executeSequence2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,109);
   aCmd->setArgDefault(2,100);
   aCmd->setArgDefault(3,10);
   aCmd->setArgDefault(4,2);

   int tId              = aCmd->argInt(1);
   int tIterations      = aCmd->argInt(2);
   int tCommandTimeout  = aCmd->argInt(3);
   int tResponseDelay   = aCmd->argInt(4);

   gMasterThread->mSequenceQCall(tId,tIterations,tCommandTimeout,tResponseDelay);
}

//******************************************************************************

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
   gMasterThread->abortWaitForNotify();
}


