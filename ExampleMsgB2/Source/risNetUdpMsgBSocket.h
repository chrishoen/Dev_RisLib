#ifndef _RISNETUDPMSGBSOCKET_H_
#define _RISNETUDPMSGBSOCKET_H_

/*==============================================================================

UdpRxMsgBSocket -- udp receive socket
UdpTxMsgBSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risSockets.h"
#include "risThreadsThreads.h"
#include "risByteContent.h"
#include "risByteMsgB.h"
#include "risByteMsgMonkey.h"

namespace Ris
{
namespace Net
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp receive message socket.
// Messages are based on the Ris::ByteContent message encapsulation scheme.

class  UdpRxMsgBSocket : public Sockets::BaseUdpSocket
{
public:
   UdpRxMsgBSocket(); 

   //------------------------------------------------------------------------
   // Socket:

   // These do socket and bind calls
   void configure(
      char*                aLocalIpAddr,
      int                  aLocalIpPort,
      Ris::BaseMsgMonkey* aMonkey);

   // This receives a message from the socket via blocking recvfrom calls.
   // It returns true if successful.
   // The recvfrom address is stored in mFromAddress.

   bool doReceiveMsg (Ris::ByteMsgB*& aMsg);

   Sockets::SocketAddress mFromAddress;

   // This is a record copier that is used to copy a record from a
   // byte buffer.It allows the doReceiveMsg method to receive and
   // extract a record from a byte buffer without the having the
   // record type visible to it.

   Ris::BaseMsgMonkey* mMonkey;

   //------------------------------------------------------------------------
   // State:

   // General purpose valid flag
   bool mValidFlag;

   // Metrics
   int mRxCount;
   int mRxLength;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp transmit message socket.
// Messages are based on the Ris::ByteContent message encapsulation scheme.

class  UdpTxMsgBSocket : public Sockets::BaseUdpSocket
{
public:
   UdpTxMsgBSocket(); 

   //------------------------------------------------------------------------
   // Socket, these two should be used together

   // These create and configure the socket
   void configure(
      char*                aRemoteIpAddr,
      int                  aRemoteIpPort,
      Ris::BaseMsgMonkey* aMonkey);

   // This sends a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.

   bool doSendMsg(Ris::ByteMsgB* aMsg);

   //------------------------------------------------------------------------
   // This is a record copier that is used to copy a record to a byte
   // buffer. It allows the doSendMsg method to send a record to a byte 
   // buffer without the having the record code visible to it.

   Ris::BaseMsgMonkey* mMonkey;

   //------------------------------------------------------------------------
   // Transmit mutex is used by doSendMsg for mutual exclusion.

   Ris::Threads::MutexSemaphore  mTxMutex;

   //------------------------------------------------------------------------
   // State:

   // General purpose valid flag
   bool mValidFlag;

   // Metrics
   int mTxCount;
   int mTxLength;
};

}//namespace
}//namespace
#endif

