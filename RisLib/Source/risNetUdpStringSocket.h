#ifndef _RISNETUDPSTRINGSOCKET_H_
#define _RISNETUDPSTRINGSOCKET_H_

/*==============================================================================

UdpRxStringSocket -- udp receive socket
UdpTxStringSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risSockets.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Udp receive message socket.
// Messages are based on the ByteContent message encapsulation scheme.

class  UdpRxStringSocket : public Sockets::BaseUdpSocket
{
public:
   UdpRxStringSocket(); 

   //--------------------------------------------------------------
   // Socket:

   // These do socket and bind calls
   void configure(int aPort);
   void configure(Sockets::SocketAddress aLocal);

   // This receives a string into the allocated receive buffer
   // It returns true if successful.
   // The recvfrom address is stored in mFromAddress.
   bool doRecvString ();

   Sockets::SocketAddress mFromAddress;

   // Buffer
   static const int cStringSize = 1000;
   char  mRxString[cStringSize];
   int   mRxLength;

   //--------------------------------------------------------------
   // State:

   // General purpose valid flag
   bool mValidFlag;

   // Metrics
   int mRxCount;
};

//******************************************************************************
// Udp transmit message socket.
// Messages are based on the ByteContent message encapsulation scheme.

class  UdpTxStringSocket : public Sockets::BaseUdpSocket
{
public:
   UdpTxStringSocket(); 

   //--------------------------------------------------------------
   // Socket, these two should be used together

   // These create and configure the socket
   void configure(int aPort);
   void configure(Sockets::SocketAddress aRemote);

   // This sends a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendString(char * aString);

   //--------------------------------------------------------------

   // Buffer
   static const int cStringSize = 1000;
   char  mTxString[cStringSize];
   int   mTxLength;

   //--------------------------------------------------------------
   // Mutex:

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore  mTxMutex;

   //--------------------------------------------------------------
   // State:

   // General purpose valid flag
   bool mValidFlag;

   // Metrics
   int mTxCount;
};
}//namespace
}//namespace
#endif

