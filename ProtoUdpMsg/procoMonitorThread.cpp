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
   ProtoComm::MsgMetrics* tTxMsgMetrics = (ProtoComm::MsgMetrics*)gProcThread->mMsgMonkey->mTxMsgMetrics;
   ProtoComm::MsgMetrics* tRxMsgMetrics = (ProtoComm::MsgMetrics*)gProcThread->mMsgMonkey->mRxMsgMetrics;

   mMon_TxMsgCount.bind(&tTxMsgMetrics->mMsgCount);
   mMon_TxByteCount.bind(&tTxMsgMetrics->mByteCount);
   mMon_RxMsgCount.bind(&tRxMsgMetrics->mMsgCount);
   mMon_RxByteCount.bind(&tRxMsgMetrics->mByteCount);
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
      ProtoComm::MsgMetrics* tTxMsgMetrics = (ProtoComm::MsgMetrics*)gProcThread->mMsgMonkey->mTxMsgMetrics;
      ProtoComm::MsgMetrics* tRxMsgMetrics = (ProtoComm::MsgMetrics*)gProcThread->mMsgMonkey->mRxMsgMetrics;

      Prn::print(Prn::Show1, "TxMsgCount               %-10d  %d",
         mMon_TxMsgCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount              %-10lld  %lld", 
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show1, "TxTestMsgCount           %-10d", tTxMsgMetrics->mTestMsgCount);
      Prn::print(Prn::Show1, "TxFirstMessageMsgCount   %-10d", tTxMsgMetrics->mFirstMessageMsgCount);
      Prn::print(Prn::Show1, "TxEchoRequestMsgCount    %-10d", tTxMsgMetrics->mEchoRequestMsgCount);
      Prn::print(Prn::Show1, "TxEchoResponseMsgCount   %-10d", tTxMsgMetrics->mEchoResponseMsgCount);
      Prn::print(Prn::Show1, "TxDataMsgCount           %-10d", tTxMsgMetrics->mDataMsgCount);
      Prn::print(Prn::Show1, "TxByteBlobMsgCount       %-10d", tTxMsgMetrics->mByteBlobMsgCount);
      
      Prn::print(Prn::Show1, "RxMsgCount               %-10d  %d",
         mMon_RxMsgCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount              %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show1, "RxTestMsgCount           %-10d", tRxMsgMetrics->mTestMsgCount);
      Prn::print(Prn::Show1, "RxFirstMessageMsgCount   %-10d", tRxMsgMetrics->mFirstMessageMsgCount);
      Prn::print(Prn::Show1, "RxEchoRequestMsgCount    %-10d", tRxMsgMetrics->mEchoRequestMsgCount);
      Prn::print(Prn::Show1, "RxEchoResponseMsgCount   %-10d", tRxMsgMetrics->mEchoResponseMsgCount);
      Prn::print(Prn::Show1, "RxDataMsgCount           %-10d", tRxMsgMetrics->mDataMsgCount);
      Prn::print(Prn::Show1, "RxByteBlobMsgCount       %-10d", tRxMsgMetrics->mByteBlobMsgCount);

      Prn::print(Prn::Show1, "");
   }

   // Show.
   if (mShowCode == 2)
   {
      ProtoComm::MsgMetrics* tTxMsgMetrics = (ProtoComm::MsgMetrics*)gProcThread->mMsgMonkey->mTxMsgMetrics;
      ProtoComm::MsgMetrics* tRxMsgMetrics = (ProtoComm::MsgMetrics*)gProcThread->mMsgMonkey->mRxMsgMetrics;
      Ris::SerialMsgPort* tMsgPort = (Ris::SerialMsgPort*)&gProcThread->mSerialMsgThread->mSerialMsgPort;

      Prn::print(Prn::Show1, "TxMsgCount               %-10d  %d",
         mMon_TxMsgCount.mValue, mMon_TxMsgCount.mDelta);
      Prn::print(Prn::Show1, "TxByteCount              %-10lld  %lld",
         mMon_TxByteCount.mValue, mMon_TxByteCount.mDelta);

      Prn::print(Prn::Show1, "RxMsgCount               %-10d  %d",
         mMon_RxMsgCount.mValue, mMon_RxMsgCount.mDelta);
      Prn::print(Prn::Show1, "RxByteCount              %-10lld  %lld",
         mMon_RxByteCount.mValue, mMon_RxByteCount.mDelta);

      Prn::print(Prn::Show1, "PortErrorCount           %-10d",
         tMsgPort->mPortErrorCount);

      Prn::print(Prn::Show1, "MsgErrorCount            %-10d",
         tMsgPort->mMsgErrorCount);

      Prn::print(Prn::Show1, "MsgResyncCount           %-10d",
         tMsgPort->mMsgResyncCount);

      Prn::print(Prn::Show1, "");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

}//namespace