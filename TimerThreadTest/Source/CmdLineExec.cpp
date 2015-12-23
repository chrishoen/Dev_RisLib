
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "prnPrint.h"

#include "CmdLineExec.h"

#include "risThreads.h"
#include "risPortableCalls.h"

#include "uviewTimeMarker.h"

#include "someTimerThread.h"
using namespace Some;

//******************************************************************************
//******************************************************************************
//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
//******************************************************************************
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("TP"    ))  executeTP      (aCmd);
   if(aCmd->isCmd("TEST"  ))  executeTest    (aCmd);
   if(aCmd->isCmd("GO1"   ))  executeGo1     (aCmd);
   if(aCmd->isCmd("GO2"   ))  executeGo2     (aCmd);
   if(aCmd->isCmd("GO3"   ))  executeGo3     (aCmd);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTP(Ris::CmdLineCmd* aCmd)
{
   gTimerThread->mTPFlag=aCmd->argBool(1);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeTest(Ris::CmdLineCmd* aCmd)
{
   gTimerThread->mTestCode=aCmd->argInt(1);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo1 (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,100000);
   int tCount = aCmd->argInt(1);

   UView::TimeMarker tMarker;
   tMarker.initialize(tCount);

   while(true)
   {
      tMarker.doStart();
      tMarker.doStop();
      if (tMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, " %10.3f  %10.3f  %10.3f  %10.3f",
      tMarker.mStatistics.mMean,
      tMarker.mStatistics.mStdDev,
      tMarker.mStatistics.mMinX,
      tMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo2 (Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,100000);
   int tCount = aCmd->argInt(1);

   UView::TimeMarker tMarker;
   tMarker.initialize(tCount);

   Ris::Threads::MutexSemaphore tMutex;

   while(true)
   {
      tMarker.doStart();

      tMutex.lock();
      tMutex.unlock();

      tMarker.doStop();
      if (tMarker.mStatistics.mEndOfPeriod)
      {
         break;
      }
   }

   Prn::print(Prn::ThreadRun1, " %10.3f  %10.3f  %10.3f  %10.3f",
      tMarker.mStatistics.mMean,
      tMarker.mStatistics.mStdDev,
      tMarker.mStatistics.mMinX,
      tMarker.mStatistics.mMaxX);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void CmdLineExec::executeGo3 (Ris::CmdLineCmd* aCmd)
{
}



