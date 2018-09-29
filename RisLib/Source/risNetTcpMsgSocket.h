#pragma once

/*==============================================================================
Tcp message socket class.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risByteContent.h"
#include "risByteMsgMonkey.h"
#include "risContainers.h"
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
// Tcp message socket. This class encapsulates a tcp stream socket that
// communicates messages that are based on the byte content message
// encapsulation scheme.
//
// For a client, it connects (connect) to a Tcp server hub socket and
// exchanges messages (send and recv) with a server stream socket.
//
// For a server, it exchanges messages (send and recv) with a client stream
// socket.
//
// It inherits from BaseTcpStreamSocket for stream socket functionality and
// provides methods that can be used to send and receive messages.
//
// Messages are based on the byte content message encapsulation scheme.

class TcpMsgSocket : public Sockets::BaseTcpStreamSocket
{
public:
   typedef Sockets::BaseTcpStreamSocket BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Settings.
   Settings* mSettings;

   // This a  message monkey that is used to get details about  a message from
   // a message header that is contained in a byte buffer. For receive, the 
   // message monkey allows the doRecvMsg method to receive and extract a
   // message from a byte buffer without the having the message code visible
   // to it. For transmit, message monkey allows the doSendMsg method to set
   // header data before the message is sent.
   BaseMsgMonkey* mMonkey;

   // Transmit mutex is used by doSendMsg for mutual exclusion.
   Threads::MutexSemaphore  mTxMutex;

   // General purpose valid flag.
   bool mValidFlag;

   // Metrics.
   int mTxMsgCount;
   int mRxMsgCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Constructor.
   TcpMsgSocket(); 
  ~TcpMsgSocket(); 

   // Initialize variables.
   void initialize(Settings* aSettings);

   // Configure the socket. This does socket and bind calls.
   void configure();

   // Reconfigure the socket. This does socket and bind calls.
   void reconfigure();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Copy a message into a byte buffer and then send the byte buffer 
   // via the socket.
   // Return true if successful.
   bool doSendMsg(ByteContent*  aTxMsg);

   // Receive a message from the socket via a blocking recv call into a
   // byte buffer and extract a message from the byte buffer.
   // Return true if successful.
   bool doReceiveMsg (ByteContent*& aRxMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

