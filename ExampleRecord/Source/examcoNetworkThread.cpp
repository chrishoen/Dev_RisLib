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
   mRxMessageQCall.bind   (this,&NetworkThread::executeRxMessage);
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
   // Configure socket thread

   mUdpRecordThread->configure(
      gSettings.mMyUdpIPAddress,
      gSettings.mMyUdpPort,
      gSettings.mOtherUdpIPAddress,
      gSettings.mOtherUdpPort,
      &mRecordCopier,
      &mRxMessageQCall);

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

void NetworkThread::executeRxMessage(Ris::ByteRecord* aRecord)
{
   // Message jump table based on message type.
   // Calls corresponding specfic message handler method.
   switch (aRecord->mRecordType)
   {
      case TypeIdT::cTestRecord :
         processRxMessage((TestRecord*)aRecord);
         break;
      case TypeIdT::cStatusRecord :
         processRxMessage((StatusRecord*)aRecord);
         break;
      case TypeIdT::cData1Record :
         processRxMessage((Data1Record*)aRecord);
         break;
      case TypeIdT::cData2Record :
         processRxMessage((Data2Record*)aRecord);
         break;
      default :
         Prn::print(Prn::ThreadRun1, "NetworkThread::executeServerRxRecord ??? %d",aRecord->mRecordType);
         delete aRecord;
         break;
   }
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(TestRecord* aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_TestRecord" );

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(StatusRecord* aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_StatusRecord %d",mStatusCount1++);

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(Data1Record* aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_Data1Record");

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// Message handler

void NetworkThread::processRxMessage(Data2Record* aRecord)
{
   Prn::print(Prn::ThreadRun1, "NetworkThread::processRxMessage_Data2Record");

   Helper::show(aRecord);

   delete aRecord;
}

//******************************************************************************
// QCall

void NetworkThread::executeOnTimer(int aTimerCount)
{
   Prn::print(Prn::ThreadRun2, "NetworkThread::executeRxMessage");

   return;

   TestRecord* tx = new TestRecord;
   tx->mCode1=101;

   sendMessage(tx);
}

//******************************************************************************
// This sends a message via the tcp client thread

void NetworkThread::sendMessage (Ris::ByteRecord* aRecord)
{
   mUdpRecordThread->sendMessage(aRecord);
}

//******************************************************************************
// This sends a test message via the tcp client thread

void NetworkThread::sendTestRecord()
{
   TestRecord* tRecord = new TestRecord;
   tRecord->mCode1=201;
 
   mUdpRecordThread->sendMessage(tRecord);
}

}//namespace