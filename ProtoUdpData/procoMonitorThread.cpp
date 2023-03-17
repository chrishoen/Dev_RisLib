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

   Ris::Net::UdpDataSocket* tDataSocket = (Ris::Net::UdpDataSocket*)&gProcThread->mUdpDataThread->mDataSocket;
   mMon_TxDataCount.bind(&tDataSocket->mTxCount);
   mMon_RxDataCount.bind(&tDataSocket->mRxCount);
}

// Update status variables.
void MonitorThread::update()
{
   mMon_TxDataCount.update();
   mMon_RxDataCount.update();
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
      Prn::print(Prn::Show2, "TxDataCount               %-10d  %d",
         mMon_TxDataCount.mValue, mMon_TxDataCount.mDelta);
      Prn::print(Prn::Show2, "TxByteCount                 %-10lld  %lld",
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show2, "RxDataCount               %-10d  %d",
         mMon_RxDataCount.mValue, mMon_RxDataCount.mDelta);
      Prn::print(Prn::Show2, "RxByteCount                 %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show2, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace