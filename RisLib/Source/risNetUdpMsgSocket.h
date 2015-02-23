#ifndef _RISNETUDPMSGSOCKET_H_
#define _RISNETUDPMSGSOCKET_H_

/*==============================================================================

UdpRxMsgSocket -- udp receive socket
UdpTxMsgSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risByteContent.h"
#include "risByteMessage.h"
#include "risSockets.h"
#include "risThreads.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Udp receive message socket.
// Messages are based on the ByteContent message encapsulation scheme.

class  UdpRxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   UdpRxMsgSocket(); 
  ~UdpRxMsgSocket(); 

   //--------------------------------------------------------------
   // Socket:

   // These do socket and bind calls
   void configure(
      Sockets::SocketAddress    aLocal,
      BaseMessageParserCreator* aMessageParserCreator);

   // This receives a message from the socket via blocking recvfrom calls.
   // It returns true if successful.
   // The recvfrom address is stored in mFromAddress.
   bool doRecvMsg (ByteContent*& aRxMsg);

   Sockets::SocketAddress mFromAddress;

   // This is a message parser that is used to get details about 
   // a message from a message header that is contained in a
   // byte buffer. It allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code
   // visible to it.
   BaseMessageParser* mMessageParser;

   // Buffers
   enum    {BUFFER_SIZE = 20000};
   char*                mRxBuffer;
   int                  mRxLength;

   //--------------------------------------------------------------
   // State:

   // General purpose valid flag
   bool mValidFlag;

   // Metrics
   int mRxMsgCount;
};

//******************************************************************************
// Udp transmit message socket.
// Messages are based on the ByteContent message encapsulation scheme.

class  UdpTxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   UdpTxMsgSocket(); 
  ~UdpTxMsgSocket(); 

   //--------------------------------------------------------------
   // Socket, these two should be used together

   // These create and configure the socket
   void configure(
      Sockets::SocketAddress      aRemote,
      BaseMessageParserCreator*   aMessageParserCreator);

   // This sends a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(
      ByteContent*  aTxMsg);

   //--------------------------------------------------------------
   // Socket, these two should be used together

   void configure(
      BaseMessageParser* aMessageParser);

   // This sends a message over the socket via a blocking sendto call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(
      Sockets::SocketAddress aRemote,
      ByteContent*           aTxMsg);

   //--------------------------------------------------------------

   // This is a message parser that is used to get details about 
   // a message from a message header that is contained in a
   // byte buffer. It allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code
   // visible to it.
   BaseMessageParser* mMessageParser;

   // Buffers
   enum    {BUFFER_SIZE = 20000};
   char*                mTxBuffer;
   int                  mTxLength;

   //--------------------------------------------------------------
   // Mutex:

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore  mTxMutex;

   //--------------------------------------------------------------
   // State:

   // General purpose valid flag
   bool mValidFlag;

   // Metrics
   int mTxMsgCount;
};
}//namespace
}//namespace
#endif

