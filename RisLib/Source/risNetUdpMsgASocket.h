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
#include "risByteMsgAParser.h"
#include "risSockets.h"
#include "risThreadsThreads.h"

namespace Ris
{
namespace Net
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Message Socket Definitions

   class MsgASocketDefT
   {
   public:
      //************************************************************************
      // Use this for a buffer size for these sockets

      static const int cBufferSize = 20000;

   };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Udp receive message socket.
   // Messages are based on the ByteContent message encapsulation scheme.

   class UdpRxMsgASocket : public Sockets::BaseUdpSocket
   {
   public:
      UdpRxMsgASocket(); 
     ~UdpRxMsgASocket(); 

     //------------------------------------------------------------------------
     // Socket:

      // These do socket and bind calls
      void configure(
         char*                     aLocalIpAddr,
         int                       aLocalIpPort,
         BaseMsgAParserCreator* aMessageParserCreator);

      // This receives a message from the socket via blocking recvfrom calls.
      // It returns true if successful.
      // The recvfrom address is stored in mFromAddress.
      bool doReceiveMessage (ByteContent*& aRxMsg);

      Sockets::SocketAddress mFromAddress;

      // This is a message parser that is used to get details about 
      // a message from a message header that is contained in a
      // byte buffer. It allows the doReceiveMessage method to receive and extract a
      // message from a byte buffer without the having the message code
      // visible to it.
      BaseMsgAParser* mMessageParser;

      // Buffer
      int    mRxLength;

      //------------------------------------------------------------------------
      // State:

      // General purpose valid flag
      bool mValidFlag;

      // Metrics
      int mRxMsgCount;
   };

   //***************************************************************************
   // Udp transmit message socket.
   // Messages are based on the ByteContent message encapsulation scheme.

   class UdpTxMsgASocket : public Sockets::BaseUdpSocket
   {
   public:
      UdpTxMsgASocket(); 
     ~UdpTxMsgASocket(); 

     //------------------------------------------------------------------------
     // Socket, these two should be used together

      // These create and configure the socket
      void configure(
         char*                       aRemoteIpAddr,
         int                         aRemoteIpPort,
         BaseMsgAParserCreator*   aMessageParserCreator);

      // This sends a message over the socket via a blocking send call.
      // It returns true if successful.
      // It is protected by the transmit mutex.
      bool doSendMessage(ByteContent* aMsg);

      //------------------------------------------------------------------------
      // Socket, these two should be used together

      void configure(
         BaseMsgAParser* aMessageParser);

      // This sends a message over the socket via a blocking sendto call.
      // It returns true if successful.
      // It is protected by the transmit mutex.

      bool doSendMessage(Sockets::SocketAddress aRemote,ByteContent* aMsg);

      //------------------------------------------------------------------------
      // This is a message parser that is used to get details about 
      // a message from a message header that is contained in a
      // byte buffer. It allows the doReceiveMessage method to receive and extract a
      // message from a byte buffer without the having the message code
      // visible to it.

      BaseMsgAParser* mMessageParser;

      //------------------------------------------------------------------------
      // Transmit mutex is used by doSendMessage for mutual exclusion.

      Threads::MutexSemaphore  mTxMutex;

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

