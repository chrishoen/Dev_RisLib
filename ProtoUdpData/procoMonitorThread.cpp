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

   // Autostart.
   mShowCode = 2;
   Prn::setFilter(Prn::Show1, false);
   Prn::setFilter(Prn::Show2, true);


   // Bind member variables.
   Ris::Net::UdpDataSocket* tSocket = &gProcThread->mUdpDataThread->mDataSocket;

   mMon_TxMsgCount.bind(&tSocket->mTxMsgCount);
   mMon_TxByteCount.bind(&tSocket->mTxByteCount);
   mMon_RxMsgCount.bind(&tSocket->mRxMsgCount);
   mMon_RxByteCount.bind(&tSocket->mRxByteCount);

}

// Update status variables.
void MonitorThread::update()
{
   mMon_TxMsgCount.update();
   mMon_TxByteCount.update();
   mMon_RxMsgCount.update();
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
      Prn::print(Prn::Show2, "TxMsgCount               %-10d  %d",
         mMon_TxMsgCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show2, "TxByteCount              %-10lld  %lld",
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show2, "RxMsgCount               %-10d  %d",
         mMon_RxMsgCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show2, "RxByteCount              %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show2, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace