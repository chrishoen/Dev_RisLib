/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "ris_priorities.h"
#include "risSerialMsgThread.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

SerialMsgThread::SerialMsgThread()
{
   BaseClass::mThreadPriority = get_default_udp_rx_thread_priority();

   mPortNumber = 0;
   mPortSetup[0]=0;
   mRxTimeout=0;
   mMonkey = 0;
   mTxCount = 0;
   mTxLength = 0;
}

SerialMsgThread::~SerialMsgThread()
{
   if (mMonkey) delete mMonkey;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Configure:

void SerialMsgThread::configure(
   Ris::BaseMsgMonkeyCreator* aMonkeyCreator, 
   int                        aPortNumber,
   char*                      aPortSetup,
   int                        aRxTimeout,
   RxMsgQCall*                aRxMsgQCall)
{
   mPortNumber = aPortNumber;
   strcpy(mPortSetup,aPortSetup);
   mRxTimeout = aRxTimeout;

   mMonkey = aMonkeyCreator->createMonkey();

   mRxMsgQCall = *aRxMsgQCall;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread init function, base class overload.
// It configures the socket.

void SerialMsgThread::threadInitFunction()
{
   Prn::print(Prn::SerialInit1, "SerialMsgThread::threadInitFunction");

   mSerialPort.doOpen(mPortNumber,mPortSetup,mRxTimeout);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread run function, base class overload.
// It contains a while loop that manages the connection to the server
// and receives messages.

void  SerialMsgThread::threadRunFunction()
{
   Prn::print(Prn::SocketRun1, "SerialRxMsgThread::threadRunFunction");
   
   //----------------------------------------------------------------------------
   // Loop
#if 0
   bool tGoing=mSerialPort.mValidFlag;

   while(tGoing)
   {
      // Try to receive a message with a blocking receive call.
      // If a message was received then process it.
      // If a message was not received then the connection was lost.  
      ByteContent* tMsg=0;
      if (mRxSocket.doReceiveMsg(tMsg))
      {
         // Message was correctly received
         // Call the receive method
         processRxMsg(tMsg);
      }
      else
      {
         // Message was not correctly received
      }
      //-------------------------------------------------------------------------
      // If termination request, exit the loop
      // This is set by shutdown, see below.
      if (mTerminateFlag)
      {
         tGoing=false;
      }  
   }         
#endif
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Thread exit function, base class overload.

void SerialMsgThread::threadExitFunction()
{
   Prn::print(Prn::SerialInit1, "SerialMsgThread::threadExitFunction");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shutdown, base class overload.
// This sets the terminate request flag and closes the socket.
//
// If the while loop in the threadRunFunction is blocked on doReceiveMsg then
// closing the socket will cause doReceiveMsg to return with false and 
// then the terminate request flag will be polled and the threadRunFunction 
// will exit.

void SerialMsgThread::shutdownThread()
{
   BaseThreadWithTermFlag::mTerminateFlag = true;

   mSerialPort.doClose();

   BaseThreadWithTermFlag::waitForThreadTerminate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Invoke the qcall callback.

void SerialMsgThread::processRxMsg(Ris::ByteContent* aMsg)
{
   // Invoke the receive qcall callback, passes the received message to the
   // thread owner.
   mRxMsgQCall(aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This sends a message via the tcp client thread

bool SerialMsgThread::sendMsg (Ris::ByteContent* aMsg)
{
   // Guard.
   if (!mSerialPort.mValidFlag) return false;

   // Create a byte buffer.
   ByteBuffer tBuffer(mMonkey->getMaxBufferSize());

   // Configure the byte buffer.
   mMonkey->configureByteBuffer(&tBuffer);
   tBuffer.setCopyTo();

   // Copy the message to the buffer.
   mMonkey->putMsgToBuffer(&tBuffer, aMsg);

   // Delete the message.
   delete aMsg;

   // Transmit the buffer.
   mTxLength = tBuffer.getLength();
   mSerialPort.doSendBytes(tBuffer.getBaseAddress(), mTxLength);

   Prn::print(Prn::SocketRun2, "SerialMsgThread tx message %d", mTxLength);

   return true;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

