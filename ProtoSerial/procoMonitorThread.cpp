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
   mMon_TxMsgCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mTxMsgCount);
   mMon_RxMsgCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mRxMsgCount);
   mMon_TxByteCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mTxByteCount);
   mMon_RxByteCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mRxByteCount);

   mMon_TxMsgCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mMonkey->mTxMsgMetrics->mMsgCount);
   mMon_RxMsgCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mMonkey->mRxMsgMetrics->mMsgCount);
   mMon_TxByteCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mMonkey->mTxMsgMetrics->mByteCount);
   mMon_RxByteCount.bind(&gProcThread->mSerialMsgThread->mSerialMsgPort.mMonkey->mRxMsgMetrics->mByteCount);

   // Set member variables.
   mTPFlag = true;
   mShowCode = 0;
}

// Update status variables.
void MonitorThread::update()
{
   mMon_TxMsgCount.update();
   mMon_RxMsgCount.update();
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
   if (mShowCode == 1)
   {
      Prn::print(Prn::Show1, "TxMsgCount               %-10d  %d",
         mMon_TxMsgCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxMsgCount               %-10d  %d", 
         mMon_RxMsgCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount              %-10lld  %lld", 
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount              %-10lld  %lld", 
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);
      Prn::print(Prn::Show1, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace