#pragma once

/*==============================================================================
Spp message socket class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risBtSettings.h"
#include "risBtSockets.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Bt
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Spp message socket. This class encapsulates a tcp stream socket that
// communicates messages that are based on the byte content message
// encapsulation scheme.
//
// For a client, it connects (connect) to a Spp server hub socket and
// exchanges messages (send and recv) with a server stream socket.
//
// For a server, it exchanges messages (send and recv) with a client stream
// socket.
//
// It inherits from BaseSppStreamSocket for stream socket functionality and
// provides methods that can be used to send and receive messages.
//
// Messages are based on the byte content message encapsulation scheme.

class SppMsgSocket : public BtSockets::BaseSppStreamSocket
{
private:
   typedef BtSockets::BaseSppStreamSocket BaseClass;
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings mSettings;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Transmit and receive memory. Allocated at initialization.
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

   // General purpose valid flag.
   bool mValidFlag;

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
   // Members.

   // Constructor.
   SppMsgSocket(); 
  ~SppMsgSocket(); 

   // Initialize variables.
   void initialize(Settings& aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure(
      Ris::BtSockets::BtSocketAddress* aRemoteAddress,
      bool aShowFlag);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy a message into a byte buffer and then send the byte buffer to the
   // socket with a blocking send call. Return true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg(ByteContent*  aTxMsg);

   // Receive a message from the socket with a blocking recv call into a
   // byte buffer and extract a message from the byte buffer. Return the
   // message and true if successful. As part of the termination process,
   // returning false means that the socket was closed or that there was
   // an error.
   bool doReceiveMsg (ByteContent*& aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

