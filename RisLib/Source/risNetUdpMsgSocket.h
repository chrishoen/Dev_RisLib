#pragma once

/*==============================================================================
UDP receive  message socket.
UDP transmit message socket.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risSockets.h"
#include "risNetSettings.h"
#include "risThreadsThreads.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Net
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp receive message socket. This class encapsulates a udp socket that
// receives messages that are based on the byte content message encapsulation
// scheme.

class UdpRxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   typedef Sockets::BaseUdpSocket BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings* mSettings;

   // The recvfrom address is stored here.
   Sockets::SocketAddress mFromAddress;

   // Number of bytes received.
   int mRxLength;

   // This is a message monkey that is used to get details about a message 
   // from a message header that is contained in a byte buffer. It allows the 
   // receive method to receive and extract a message from a byte buffer
   // without the having the message code visible to it.
   BaseMsgMonkey* mMonkey;

   // True if the socket is valid.
   bool mValidFlag;

   // Metrics.
   int mRxCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpRxMsgSocket(); 
   ~UdpRxMsgSocket(); 

   // Initialize variables.
   void initialize(Settings* aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive a message from the socket via blocking recvfrom calls.
   // Return true if successful.
   bool doReceiveMsg (ByteContent*& aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Udp transmit message socket. This class encapsulates a udp socket that
// transmits messages that are based on the byte content message encapsulation
// scheme.

class UdpTxMsgSocket : public Sockets::BaseUdpSocket
{
public:
   typedef Sockets::BaseUdpSocket BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings* mSettings;

   // This is a message monkey that is used to get details about a message 
   // from a message header that is contained in a byte buffer. It allows the 
   // receive method to receive and extract a message from a byte buffer
   // without the having the message code visible to it.
   BaseMsgMonkey* mMonkey;

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore  mTxMutex;

   // General purpose valid flag.
   bool mValidFlag;

   // Metrics.
   int mTxCount;
   int mTxLength;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpTxMsgSocket(); 
   ~UdpTxMsgSocket(); 

   // Initialize variables.
   void initialize(Settings* aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Send a message over the socket via a blocking send call.
   // Return true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


