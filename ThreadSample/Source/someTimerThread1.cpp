/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <windows.h>
#include "prnPrint.h"

#include "someClass1.h"
#include "GSettings.h"
#include "someQCallThread1.h"

#define  _SOMETIMERTHREAD1_CPP_
#include "someTimerThread1.h"

namespace Some
{

TimerThread1::TimerThread1()
{
   BaseClass::setThreadPriorityHigh();
   mTimerPeriod = 1000;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void TimerThread1::executeOnTimer(int aTimeCount)
{
   //---------------------------------------------------------------------------
   // Time marker

   //---------------------------------------------------------------------------
   switch (gGSettings.mTestThread)
   {
      //------------------------------------------------------------------------
      case GSettings::cTestThread_QCallThread1:
      {
         switch (gGSettings.mTestNumber)
         {
            case 1:
            {
               gQCallThread1->mC101QCall(aTimeCount);
            }
            break;
         }
      }
      break;
   }
}

}//namespace