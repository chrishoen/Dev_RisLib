/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include "risSockets.h"
#include "prnPrint.h"
#include "examcoSettings.h"
#include "examcoHelper.h"

#define  _EXAMCONETWORKTHREAD_CPP_
#include "examcoNetworkThread.h"


namespace ExampleComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

NetworkThread::NetworkThread()
{
   mStatusCount1=0;
   mStatusCount2=0;

   mUdpRecordThread = new Ris::Net::UdpRecordThread;

   // Initialize QCalls
   mRxRecordQCall.bind   (this,&NetworkThread::executeRxRecord);
}

//******************************************************************************

NetworkThread::~NetworkThread()
{
   delete mUdpRecordThread;
}

//******************************************************************************
// This configures members

void NetworkThread::configure()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::configure");

   //---------------------------------------------------------------------------
   // Configure receive socket thread

   mUdpRecordThread->configure(
      gSettings.mMyUdpIPAddress,
      gSettings.mMyUdpPort,
      &mRecordCopier,
      &mRxRecordQCall);

   //---------------------------------------------------------------------------
   // Configure transmit socket

   mTxSocket.configure(
      gSettings.mOtherUdpIPAddress,
      gSettings.mOtherUdpPort,
      &mRecordCopier);
}

//******************************************************************************

void NetworkThread::launchThread()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::launch");

   // Launch child thread
   mUdpRecordThread->launchThread(); 
   
   // Launch this thread
   BaseClass::launchThread();
}

//******************************************************************************
// Thread exit function, base class overload.
void  NetworkThread::threadExitFunction()
{
   Prn::print(Prn::ThreadInit1, "NetworkThread::threadExitFunction");

   // Shutdown the tcp client thread
   mUdpRecordThread->shutdownThread();

   // Base class exit
   BaseClass::threadExitFunction();
}

//******************************************************************************
// QCall

void NetworkThread::executeRxRecord(Ris::ByteRecord* aRecord)
{
   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (aRecord->mRecordType)
   {
      case TypeIdT::cTestRecord :
         processRxRecord((TestRecord*)aRecord);
         break;
      case TypeIdT::cStatusRecord :
         processRxRecord((StatusRecord*)aRecord);
         break;
      case TypeIdT::cData1Record :
         processRxRecord((Data1Record*)aRecord);
         break;
      case TypeIdT::cData2Record :
         processRxRecord((Data2Record*)aRecord);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxRecord ??? %d",aRecord->mRecordType);
         delete aRecord;
         break;
   }
}

//******************************************************************************
// Message handler

void NetworkThread::processRxRecord(TestRecord*  aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxRecord_TestRecord" );

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxRecord(StatusRecord* aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxRecord_StatusRecord %d",mStatusCount1++);

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxRecord(Data1Record* aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxRecord_Data1Record");

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxRecord(Data2Record* aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxRecord_Data2Record");

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// QCall

void NetworkThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun2, "NetworkThread::executeRxRecord");

   return;

   TestRecord* tx = new TestRecord;
   tx->mCode1=101;

   sendRecord(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendRecord (Ris::ByteRecord* aRecord)
{
   mTxSocket.doSendRecord(aRecord);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestRecord()
{
   TestRecord* msg = new TestRecord;
   msg->mCode1=201;
 
   mTxSocket.doSendRecord(msg);
}

}//namespace