#ifndef _RISNETUDPMSGBSOCKET_H_
#define _RISNETUDPMSGBSOCKET_H_

/*==============================================================================

UdpRxMsgBSocket -- udp receive socket
UdpTxMsgBSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>
#include "prnPrint.h"
#include "risThreadsThreads.h"

#include "risByteMsgB.h"
#include "risByteMsgBCopier.h"
#include "risByteContent.h"
#include "risSockets.h"
#include "risNetUdpMsgBHeader.h"

namespace Ris
{
namespace Net
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Definitions

class MsgBSocketDefT
{
public:
   //***************************************************************************
   // Use this for a buffer size for these sockets

   static const int cBufferSize = 20000;

};


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp receive message socket. Messages are based on the Ris::ByteContent
// message encapsulation scheme.

class UdpRxMsgBSocket : public Sockets::BaseUdpSocket
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This is a record copier that is used to copy a record from a
   // byte buffer.It allows the doReceiveMsg method to receive and
   // extract a record from a byte buffer without the having the
   // record type visible to it.
   Ris::BaseMsgBCopier* mMsgCopier;

   // General purpose valid flag
   bool mValidFlag;

   // Address of last sender that was received from.
   Sockets::SocketAddress mFromAddress;

   // Metrics
   int mRxCount;
   int mRxLength;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constructor.

   UdpRxMsgBSocket()
   {
      mRxLength     = 0;
      mRxCount      = 0;
      mValidFlag    = false;
      mMsgCopier    = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure.

   void configure(
      char*                aLocalIpAddr,
      int                  aLocalIpPort,
      Ris::BaseMsgBCopier* aMsgCopier)
   {
      mRxCount=0;

      mLocal.set(aLocalIpAddr,aLocalIpPort);
      mMsgCopier = aMsgCopier;

      doSocket();
      doBind();

      if (mStatus==0)
      {
         Prn::print(Prn::SocketInit2, "UdpRxMsgBSocket     $ %16s : %d",
            mLocal.mIpAddr.mString,
            mLocal.mPort);
      }
      else
      {
         Prn::print(Prn::SocketInit2, "UdpRxMsgBSocket     $ %16s : %d $ %d %d",
            mLocal.mIpAddr.mString,
            mLocal.mPort,
            mStatus,
            mError);
      }

      mValidFlag=mStatus==0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This receives a message from the socket via blocking recvfrom calls.
   // It returns true if successful. The recvfrom address is stored in
   // mFromAddress.

   bool doReceiveMsg (Ris::ByteMsgB*& aMsg)
   {
      //------------------------------------------------------------------------
      // Initialize
      aMsg=0;

      // Guard
      if (!mValidFlag) return false;

      // Byte buffer, constructor takes size
      Ris::ByteBuffer tBuffer(MsgBSocketDefT::cBufferSize);  
      tBuffer.setCopyFrom();

      //------------------------------------------------------------------------
      // Read the message into the receive buffer

      doRecvFrom  (mFromAddress,tBuffer.getBaseAddress(),mRxLength,MsgBSocketDefT::cBufferSize);

      // Guard
      // If bad status then return false.
      // Returning true  means socket was not closed
      // Returning false means socket was closed

      if (mRxLength<=0)
      {
         if (mStatus<0)
            switch (mError)
            {
               case 0              : return false  ;break;
               default             : return false  ;break;
            }   
         else
            return false;
      }

      // Set the buffer length
      tBuffer.setLength(mRxLength);
   
      //------------------------------------------------------------------------
      // Copy from the receive buffer into an instance of the header
      // and validate the header

      MsgBHeader tHeader;

      tBuffer.setCopyFrom();
      tBuffer.getFromBuffer(&tHeader);

      // If the header is not valid then error
      
      if (!tHeader.validate())
      {
         Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER ");
         return false;
      }

      //------------------------------------------------------------------------
      // At this point the buffer contains the complete message.
      // Extract the message from the byte buffer into a new message
      // object and return it.

      // Create a record based on the record type
      aMsg = mMsgCopier->createMessage(tHeader.mMessageIdentifier);

      // Copy from the buffer into the record
      mMsgCopier->copyToFrom(&tBuffer, aMsg);

      // Test for errors and return.
      // If the pointer is zero then message is bad
      if (aMsg==0)
      {
         mStatus=tBuffer.getError();
      }

      // Returning true  means socket was not closed
      // Returning false means socket was closed
      mRxCount++;
      return true;
   }
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp transmit message socket.
// Messages are based on the Ris::ByteContent message encapsulation scheme.

class UdpTxMsgBSocket : public Sockets::BaseUdpSocket
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This is a record copier that is used to copy a record to a byte
   // buffer. It allows the doSendMsg method to send a record to a byte 
   // buffer without the having the record code visible to it.

   Ris::BaseMsgBCopier* mMsgCopier;

   // Transmit mutex is used by doSendMsg for mutual exclusion.

   Ris::Threads::MutexSemaphore  mTxMutex;

   // General purpose valid flag
   bool mValidFlag;

   // Metrics
   int mTxCount;
   int mTxLength;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constructor.

   UdpTxMsgBSocket()
   {
      mTxCount      = 0;
      mTxLength     = 0;
      mValidFlag    = false;
      mMsgCopier = 0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Configure.

   void configure(
      char*                aRemoteIpAddr,
      int                  aRemoteIpPort,
      Ris::BaseMsgBCopier* aMsgCopier)
   {
      mTxCount=0;

      mRemote.set(aRemoteIpAddr,aRemoteIpPort);
      mMsgCopier = aMsgCopier;

      doSocket();

      if (mStatus==0)
      {
         Prn::print(Prn::SocketInit2, "UdpTxMsgBSocket     $ %16s : %d",
            mRemote.mIpAddr.mString,
            mRemote.mPort);
      }
      else
      {
         Prn::print(Prn::SocketInit2, "UdpTxMsgBSocket     $ %16s : %d $ %d %d",
            mRemote.mIpAddr.mString,
            mRemote.mPort,
            mStatus,
            mError);
      }

      mValidFlag=mStatus==0;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This sends a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.

   bool doSendMsg(Ris::ByteMsgB* aMsg)
   {
      // Guard
      if (!mValidFlag) return false;

      // Create byte buffer, constructor takes size
      Ris::ByteBuffer tBuffer(MsgBSocketDefT::cBufferSize);

      //------------------------------------------------------------------------
      // Instance of a header,set members

      MsgBHeader tHeader;
      tHeader.mMessageIdentifier = aMsg->mMessageType;

      //------------------------------------------------------------------------
      // Copy

      // Copy header to buffer
      tBuffer.setCopyTo();
      tHeader.headerCopyToFrom(&tBuffer,aMsg);

      // Copy record to buffer
      mMsgCopier->copyToFrom(&tBuffer,aMsg);
      
      // ReCopy header to buffer
      tHeader.headerReCopyToFrom(&tBuffer,aMsg);

      // Delete the record
      delete aMsg;

      //------------------------------------------------------------------------
      // Send buffer to socket

      // Mutex
      mTxMutex.lock();

      // Transmit the buffer
      mTxLength=tBuffer.getLength();
      doSendTo(mRemote,tBuffer.getBaseAddress(),mTxLength);
      Prn::print(Prn::SocketRun1, "UdpTxMsgBSocket  doSendTo   $ %d",mTxLength);

      mTxCount++;

      // Mutex
      mTxMutex.unlock();

      return true;
   }

};

}//namespace
}//namespace
#endif

