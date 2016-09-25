#ifndef _RISNETUDPMSGTHREAD_H_
#define _RISNETUDPMSGTHREAD_H_

/*==============================================================================
Udp receiver thread.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risCallPointer.h"
#include "risContainers.h"
#include "risSockets.h"
#include "risThreadsThreads.h"
#include "risThreadsQCallThread.h"
#include "risNetUdpMsgSocket.h"

namespace Ris
{
namespace Net
{

//******************************************************************************
// Udp message thread.
//
// This is a thread that provides the execution context for a udp peer that 
// communicates with another udp peer.
//
// It contains a receive  udp socket that is bound to a local address.
// It contains a transmit udp socket that is bound to a remote address.
//
// The thread is structured around a while loop that does a recvfrom
// call to receive a message on the socket.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// recv call.

class UdpMsgThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   UdpMsgThread();

   //--------------------------------------------------------------
   // Thread base class overloads:

   void threadInitFunction(); 
   void threadRunFunction(); 
   void threadExitFunction(); 
   void shutdownThread(); 

   //---------------------------------------------------------------------------
   // Configure:

   // aMonkeyCreator  creates a message monkey to be used on messages
   // aLocalIpAddr    is the ip address of the local  interface bound to
   // aLocalIpPort    is the ip port    of the local  interface bound to
   // aRemoteIpAddr   is the ip address of the remote interface bound to
   // aRemoteIpPort   is the ip port    of the remote interface bound to
   // aRxMsgQCall     is a qcall for receive messages

   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   void configure(
      Ris::BaseMsgMonkeyCreator* aMonkeyCreator, 
      char*                      aLocalIpAddress,
      int                        aLocalIpPort,
      char*                      aRemoteIpAddress,
      int                        aRemoteIpPort,
      RxMsgQCall*                aRxMsgQCall);

   //---------------------------------------------------------------------------
   // Process:
   
   // This is called by the UdpMsgThread threadRunFunction to process a 
   // received message.
   //
   // It invokes the mRxMsgQCall that is passed in at configure.
   void processRxMsg (Ris::ByteContent* aMsg);

   //---------------------------------------------------------------------------
   // QCall:

   // This is a qcall that is called when a message is received.
   RxMsgQCall mRxMsgQCall;

   //---------------------------------------------------------------------------
   // Transmit message:

   void sendMsg (Ris::ByteContent* aMsg);

   //---------------------------------------------------------------------------
   // Sockets:

   // Socket address that socket instance connects to.
   char  mLocalIpAddress[40];
   int   mLocalIpPort;
   char  mRemoteIpAddress[40];
   int   mRemoteIpPort;

   // Socket instance
   UdpRxMsgSocket mRxSocket;
   UdpTxMsgSocket mTxSocket;

   // Message monkey creator, this is used by the two sockets to
   // create an instance of a message monkey.
   BaseMsgMonkeyCreator* mMonkeyCreator;
};

//******************************************************************************
}//namespace
}//namespace
#endif

