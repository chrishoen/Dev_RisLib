
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "CmdLineExec.h"

#include "risTimeMarker.h"
#include "risPortableCalls.h"

#include "someThread1.h"
#include "someThread2.h"
#include "someTimerThread.h"
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
   if(aCmd->isCmd("TP"    ))  executeTP      (aCmd);
   if(aCmd->isCmd("ABORT" ))  executeAbort   (aCmd);
   if(aCmd->isCmd("F101"  ))  executeF101    (aCmd);
   if(aCmd->isCmd("F102"  ))  executeF102    (aCmd);
   if(aCmd->isCmd("T1"    ))  executeTest1   (aCmd);
   if(aCmd->isCmd("T2"    ))  executeTest2   (aCmd);
   if(aCmd->isCmd("T3"    ))  executeTest3   (aCmd);
   if(aCmd->isCmd("T4"    ))  executeTest4   (aCmd);
   if(aCmd->isCmd("T5"    ))  executeTest5   (aCmd);
   if(aCmd->isCmd("M1"    ))  executeMarker1 (aCmd);
   if(aCmd->isCmd("M2"    ))  executeMarker2 (aCmd);
}

//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   gThread1->mTPFlag=aCmd->argBool(1);
}

//******************************************************************************

void CmdLineExec::executeAbort(Ris::CmdLineCmd* aCmd)
{
   gThread1->threadAbortTimerCompletion();
}

//******************************************************************************

void CmdLineExec::executeF101(Ris::CmdLineCmd* aCmd)
{
   gThread1->mF101QCall.invoke(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeF102(Ris::CmdLineCmd* aCmd)
{
   gThread1->mF102QCall.invoke(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeTest1(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,10);
   gThread2->mTest1QCall.invoke(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeTest2(Ris::CmdLineCmd* aCmd)
{
   gThread1->mTest2QCall.invoke(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeTest3(Ris::CmdLineCmd* aCmd)
{
   gThread1->mTest3QCall.invoke(aCmd->argInt(1));
}

//******************************************************************************

void CmdLineExec::executeTest4(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1000);
   aCmd->setArgDefault(2,10);
   gThread1->mTest4QCall.invoke(aCmd->argInt(1),aCmd->argInt(2));
}

//******************************************************************************

void CmdLineExec::executeTest5(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1000);
   aCmd->setArgDefault(2,10);
   gTimerThread->mTimerTester.startTest(1000,aCmd->argInt(1),aCmd->argInt(2));
}

//******************************************************************************

void CmdLineExec::executeMarker1(Ris::CmdLineCmd* aCmd)
{
   Ris::TimeMarker tMarker;
   tMarker.doBegin();
   Ris::portableSleep(1000);
   tMarker.doEnd();
   Prn::print(0,0,"%s",tMarker.asString2());
}

//******************************************************************************

void CmdLineExec::executeMarker2(Ris::CmdLineCmd* aCmd)
{
   Ris::Int64 tFreq  = Ris::portableGetHiResFrequency();
   Ris::Int64 tCount = Ris::portableGetHiResCounter();
   Prn::print(0,0,"%lld",tFreq);
   Prn::print(0,0,"%lld",tCount);
}


