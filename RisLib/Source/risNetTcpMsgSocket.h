#ifndef _RISNETTCPMSGSOCKET_H_
#define _RISNETTCPMSGSOCKET_H_

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
#include "risThreadsThreads.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Tcp stream socket, blocking.
//
// For a client, it connects (connect) to a Tcp server hub socket and
// exchanges messages (send and recv) with a server stream socket.
//
// For a server, it exchanges messages (send and recv) with a client stream
// socket.
//
// It inherits from BaseTcpStreamSocket for stream socket functionality and
// provides methods that can be used to transport messages.
//
// Messages are based on the ByteContent message encapsulation scheme.

class TcpMsgSocket : public Sockets::BaseTcpStreamSocket
{
public:
   TcpMsgSocket(); 
  ~TcpMsgSocket(); 

   //--------------------------------------------------------------
   // Socket:

   // These do socket and bind calls
   void configure(
      BaseMsgMonkeyCreator*  aMonkeyCreator,
      Sockets::SocketAddress aSocketAddress);

   void configure(
      BaseMsgMonkeyCreator*  aMonkeyCreator);

   void reconfigure(); 

   // This receives a message from the socket via blocking recv calls.
   // It returns true if successful.
   bool doReceiveMsg (ByteContent*& aRxMsg);

   // This sends a message over the socket via a blocking send call.
   // It returns true if successful.
   // It is protected by the transmit mutex.
   bool doSendMsg (ByteContent*  aTxMsg);

   // These are message monkeys that are used to get details about 
   // a message from a message header that is contained in a
   // byte buffer. The receive message monkey allows the doRecvMsg
   // method to receive and extract a message from a byte buffer 
   // without the having the message code visible to it. The transmit
   // message monkey allows the doSendMsg method to set header data
   // before the message is sent.

   BaseMsgMonkey* mMonkey;

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
   int mRxMsgCount;
};
}//namespace
}//namespace
#endif
