#pragma once

/*==============================================================================
UDP message socket.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risNetSettings.h"
#include "risSockets.h"

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
// Udp message socket. This class encapsulates a udp socket that sends
// and receives messages that are based on the byte content message
// encapsulation scheme.

class UdpMsgSocket : public Sockets::BaseUdpSocket
{
private:
   typedef Sockets::BaseUdpSocket BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Network settings.
   Settings mSettings;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Receive memory. Allocated at initialization.
   char* mRxMemory;

   // Size of allocated memory.
   int mMemorySize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This a message monkey that is used to manage the details about messages
   // and message headers while hiding the underlying specific message set code.
   // For received  messages, the message monkey allows the receive method to
   // extract message header details from a byte buffer and it allows it to 
   // then extract full messages from the byte buffer. For transmited messages,
   // the message monkey allows the send method to set header data before the
   // message is sent. A specific message monkey is provided by the parent 
   // thread at initialization.
   BaseMsgMonkey* mMsgMonkey;

   // True if the socket is valid.
   bool mValidFlag;

   // The recvfrom address from the last receive call.
   Sockets::SocketAddress mFromAddress;

   // Metrics.
   int mTxLength;
   int mRxLength;
   int mTxCount;
   int mRxCount;

   // Program trace index.
   int mTI;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   UdpMsgSocket(); 
   ~UdpMsgSocket(); 

   // Initialize variables and allocate memory.
   void initialize(Settings* aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Receive a message from the socket with a blocking recv call into a
   // byte buffer and then extract a message from the byte buffer. Return
   // the message and true if successful.
   bool doReceiveMsg (ByteContent*& aRxMsg);

   // Copy a message into a byte buffer and then send the byte buffer
   // to the socket with a send call. Return true if successful.
   bool doSendMsg(ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace


