#pragma once

/*==============================================================================
Serial I/O message thread.
==========================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsThreads.h"
#include "risThreadsQCall.h"
#include "risSerialMsgPort.h"
#include "risByteContent.h"
#include "risByteMsgMonkey.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Serial message thread.
//
// This is a thread that provides the execution context for a serial peer
// that communicates with another udp peer.
//
// It contains a serial port local com port.
//
// The thread is structured around a while loop that does a read
// call to receive a message on the serial port.
//
// The thread provides serialized access to the socket and associated 
// state variables and it provides the context for the blocking of the 
// read call.

class SerialMsgThread : public Ris::Threads::BaseThreadWithTermFlag
{
public:
   typedef Ris::Threads::BaseThreadWithTermFlag BaseClass;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This is a qcall callback that is called when a message is received.
   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   // This is a qcall callback that is called when a message is received.
   RxMsgQCall mRxMsgQCall;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Serial port configuration parameters.
   int       mPortNumber;
   char      mPortSetup[16];
   int       mRxTimeout;

   // Serial port.
   SerialMsgPort mSerialMsgPort;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // This is a message monkey that is used to get details about a message 
   // from a message header that is contained in a byte buffer. It allows the 
   // receive method to receive and extract a message from a byte buffer
   // without the having the message code visible to it.
   BaseMsgMonkeyCreator* mMonkeyCreator;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Metrics.
   int mTxCount;
   int mTxLength;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastrcture.

   // Constructor.
   SerialMsgThread();
   ~SerialMsgThread();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Configure the thread.
   // aMonkeyCreator  creates a message monkey to be used on messages
   // PortNumber  is the com port number
   // PortSetup   is the com port setup, ie 9600N81
   // RxTimeout   is the com port receive timeout, zero means no timeout
   // aRxMsgQCall     is a qcall for receive messages

   typedef Ris::Threads::QCall1<Ris::ByteContent*> RxMsgQCall;

   void configure(
      Ris::BaseMsgMonkeyCreator* aMonkeyCreator, 
      int                        aPortNumber,
      char*                      aPortSetup,
      int                        aRxTimeout,
      RxMsgQCall*                aRxMsgQCall);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, thread base class overloads:

   // Setup the socket.
   void threadInitFunction()override;

   // Execute a while loop that does recv calls. The loop exits
   // when the socket is closed and the termination flag is true.
   void threadRunFunction()override;

   // Print.
   void threadExitFunction()override;

   // Set the termination flag, close the socket and wait for the thread to
   // terminate.
   void shutdownThread()override; 

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Process a received message. This is called by the threadRunFunction.
   // It invokes the mRxMsgQCall that is passed in at configure to pass the
   // message to the thread owner.
   void processRxMsg (Ris::ByteContent* aMsg);

   // Send a transmit message through the serial port. It executes a blocking
   // send call in the context of the caller.
   bool sendMsg (Ris::ByteContent* aMsg);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

