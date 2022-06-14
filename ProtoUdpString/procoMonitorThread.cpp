/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "stdafx.h"

#include "procoProcThread.h"

#define  _PROCOMONITORTHREAD_CPP_
#include "procoMonitorThread.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructor.

MonitorThread::MonitorThread()
{
   // Set base class variables.
   BaseClass::setThreadName("Monitor");
   BaseClass::setThreadPriorityLow();
   BaseClass::mTimerPeriod = 1000;

   // Set member variables.
   mTPFlag = true;
   mShowCode = 0;

   // Bind member variables.

   Ris::Net::UdpStringSocket* tStringSocket = (Ris::Net::UdpStringSocket*)&gProcThread->mUdpStringThread->mStringSocket;
   mMon_TxStringCount.bind(&tStringSocket->mTxCount);
   mMon_RxStringCount.bind(&tStringSocket->mRxCount);
}

// Update status variables.
void MonitorThread::update()
{
   mMon_TxStringCount.update();
   mMon_RxStringCount.update();
   mMon_TxByteCount.update();
   mMon_RxByteCount.update();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute periodically. This is called by the base class timer.

void MonitorThread::executeOnTimer(int aTimeCount)
{
   // Update state status variables.
   update();

   // Show.
   if (mShowCode == 2)
   {
      Prn::print(Prn::Show2, "TxStringCount               %-10d  %d",
         mMon_TxStringCount.mValue, mMon_TxStringCount.mDelta);
      Prn::print(Prn::Show2, "TxByteCount                 %-10lld  %lld",
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show2, "RxStringCount               %-10d  %d",
         mMon_RxStringCount.mValue, mMon_RxStringCount.mDelta);
      Prn::print(Prn::Show2, "RxByteCount                 %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show2, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace