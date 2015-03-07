
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "CmdLineExec.h"

#include "risTimeMarker.h"
#include "risPortableCalls.h"

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
   if(aCmd->isCmd("T5"    ))  executeTest5   (aCmd);
   if(aCmd->isCmd("M1"    ))  executeMarker1 (aCmd);
   if(aCmd->isCmd("M2"    ))  executeMarker2 (aCmd);
}

//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   gTimerThread->mTPFlag=aCmd->argBool(1);
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
   long long tFreq  = Ris::portableGetHiResFrequency();
   long long tCount = Ris::portableGetHiResCounter();
   Prn::print(0,0,"%lld",tFreq);
   Prn::print(0,0,"%lld",tCount);
}


