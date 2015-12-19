#ifndef _RISNETUDPRECORDSOCKET_H_
#define _RISNETUDPRECORDSOCKET_H_

/*==============================================================================

UdpRxRecordSocket -- udp receive socket
UdpTxRecordSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteRecord.h"
#include "risByteRecordCopier.h"
#include "risSockets.h"
#include "risThreads.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Udp receive message socket.
// Messages are based on the ByteContent message encapsulation scheme.

class  UdpRxRecordSocket : public Sockets::BaseUdpSocket
{
public:
   UdpRxRecordSocket(); 
  ~UdpRxRecordSocket(); 

   //--------------------------------------------------------------
   // Socket:

   // These do socket and bind calls
   void configure(
      Sockets::SocketAddress    aLocal,
      BaseRecordCopier* aRecordCopierCreator);

   // This receives a message from the socket via blocking recvfrom calls.
   // It returns true if successful.
   // The recvfrom address is stored in mFromAddress.
   bool doRecvMsg (ByteRecord*& aRxMsg);

   Sockets::SocketAddress mFromAddress;

   // This is a message parser that is used to get details about 
   // a message from a message header that is contained in a
   // byte buffer. It allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code
   // visible to it.
   BaseRecordCopier* mRecordCopier;

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

class  UdpTxRecordSocket : public Sockets::BaseUdpSocket
{
public:
   UdpTxRecordSocket(); 
  ~UdpTxRecordSocket(); 

   //--------------------------------------------------------------
   // Socket, these two should be used together

   // These create and configure the socket
   void configure(
      Sockets::SocketAddress      aRemote,
      BaseRecordCopier*   aRecordCopier);

   // This sends a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(
      ByteRecord*  aTxMsg);

   //--------------------------------------------------------------

   // This is a message parser that is used to get details about 
   // a message from a message header that is contained in a
   // byte buffer. It allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code
   // visible to it.
   BaseRecordCopier* mRecordCopier;

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

