#ifndef _RISNETUDPMSGSOCKET_H_
#define _RISNETUDPMSGSOCKET_H_

/*==============================================================================

UdpRxMsgSocket -- udp receive socket
UdpTxMsgSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risSockets.h"
#include "risThreadsThreads.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp receive message socket.
// Messages are based on the ByteContent message encapsulation scheme.

class UdpRxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   UdpRxMsgSocket(); 
   ~UdpRxMsgSocket(); 

   //---------------------------------------------------------------------------
   // Socket:

   // Configure the socket. This does socket and bind calls.
   void configure(
      BaseMsgMonkeyCreator* aMonkeyCreator,
      char*                 aLocalIpAddr,
      int                   aLocalIpPort);

   // Receive a message from the socket via blocking recvfrom calls.
   // Return true if successful.
   bool doReceiveMsg (ByteContent*& aRxMsg);

   // The recvfrom address is stored here.
   Sockets::SocketAddress mFromAddress;

   // Number of bytes received.
   int mRxLength;

   //---------------------------------------------------------------------------
   // This is a message monkey that is used to get details about a message 
   // from a message header that is contained in a byte buffer. It allows the 
   // receive method to receive and extract a message from a byte buffer
   // without the having the message code visible to it.
   BaseMsgMonkey* mMonkey;

   //--------------------------------------------------------------------------
   // State:

   // True if the socket is valid.
   bool mValidFlag;

   // Metrics
   int mRxMsgCount;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp transmit message socket.
// Messages are based on the ByteContent message encapsulation scheme.

class UdpTxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   UdpTxMsgSocket(); 
   ~UdpTxMsgSocket(); 

   //---------------------------------------------------------------------------
   // Socket:

   // Configure the socket. This does socket and bind calls.
   void configure(
      BaseMsgMonkeyCreator* aMonkeyCreator,
      char*                 aRemoteIpAddr,
      int                   aRemoteIpPort);

   // Send a message over the socket via a blocking send call.
   // Return true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(ByteContent* aMsg);

   //---------------------------------------------------------------------------
   // This is a message monkey that is used to get details about a message 
   // from a message header that is contained in a byte buffer. It allows the 
   // receive method to receive and extract a message from a byte buffer
   // without the having the message code visible to it.
   BaseMsgMonkey* mMonkey;

   //---------------------------------------------------------------------------
   // Transmit mutex is used by doSendMsg for mutual exclusion.

   Threads::MutexSemaphore  mTxMutex;

   //---------------------------------------------------------------------------
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

