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

   // Bind member variables.
   mMon_RxMsgCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mRxMsgCount);
   mMon_TxMsgCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mTxMsgCount);
   mMon_RxByteCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mRxByteCount);
   mMon_TxByteCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mTxByteCount);

   // Set member variables.
   mTPFlag = true;
   mShowCode = 0;
}

// Update status variables.
void MonitorThread::update()
{
   mMon_RxMsgCount.update();
   mMon_TxMsgCount.update();
   mMon_RxByteCount.update();
   mMon_TxByteCount.update();
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
   if (mShowCode == 1)
   {
      Prn::print(Prn::Show1, "RxMsgCount               %-10d  %d", mMon_RxByteCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxMsgCount               %-10d  %d", mMon_TxByteCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount              %-10d  %d", mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount              %-10d  %d", mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);
      Prn::print(Prn::Show1, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace