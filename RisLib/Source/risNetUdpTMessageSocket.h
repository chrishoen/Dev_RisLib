#ifndef _RISNETUDPTMESSAGESOCKET_H_
#define _RISNETUDPTMESSAGESOCKET_H_

/*==============================================================================

UdpRxTMessageSocket -- udp receive socket
UdpTxTMessageSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteTMessage.h"
#include "risByteTMessageCopier.h"
#include "risByteContent.h"
#include "risSockets.h"
#include "risThreadsThreads.h"

namespace Ris
{
namespace Net
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Definitions

   class TMessageSocketDefT
   {
   public:
      //************************************************************************
      // Use this for a buffer size for these sockets

      static const int cBufferSize = 20000;

   };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This encapsualtes the message header.

   class TMessageHeader : public Ris::ByteContent
   {
   public:
      // Constructor
      TMessageHeader();
      void reset();

      //------------------------------------------------------------------------
      // Header Content

      int   mSyncWord1;
      int   mSyncWord2;
      int   mMessageIdentifier;
      int   mMessageLength;
      int   mSourceId;
      int   mDestinationId;

      // Header Content
      //------------------------------------------------------------------------

      // Header length
      static const int cLength = 24;

      // Valid
      bool mHeaderValidFlag;

      //------------------------------------------------------------------------
      // Validate a received header

      bool validate();

      //------------------------------------------------------------------------
      // If the byte buffer is configured for put operations then this puts the
      // contents of the object into the byte buffer (it does a copy to, it
      // copies the object to the byte buffer).
      // If the byte buffer is configured for get operations then this gets the
      // contents of the object from the byte buffer (it does a copy from, it
      // copies the object from the byte buffer).
      // Copy To and Copy From are symmetrical.
      //------------------------------------------------------------------------

      void copyToFrom(Ris::ByteBuffer* aBuffer);

      //------------------------------------------------------------------------
      // For variable content messages, the message length cannot be known until
      // the entire message has been written to a byte buffer. Therefore, the 
      // message header cannot be written to a byte buffer until the entire
      // message has been written and the length is known.
      //
      // The procedure to write a message to a byte buffer is to skip over the 
      // buffer segment where the header is located, write the message payload
      // to the buffer, set the header message length based on the now known
      // payload length, and write the header to the buffer.
      //
      // These are called explicitly by inheriting messages at the
      // beginning and end of their copyToFrom's to manage the headers.
      // For "get" operations, headerCopyToFrom "gets" the header and
      // headerReCopyToFrom does nothing. For "put" operations,
      // headerCopyToFrom stores the buffer pointer and advances past where the
      // header will be written and headerReCopyToFrom "puts" the header at the
      // stored position. Both functions are passed a byte buffer pointer to
      // where the copy is to take place. Both are also passed a Ris::ByteContent
      // pointer to where they can get and mMessageType
      // which they transfer into and out of the headers.
      //------------------------------------------------------------------------

      void headerCopyToFrom   (Ris::ByteBuffer* aBuffer, Ris::ByteTMessage* aParent);
      void headerReCopyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteTMessage* aParent);

      //------------------------------------------------------------------------
      // These are set by headerCopyToFrom and used by headerReCopyToFrom,
      // for "put" operations.Theyt contain the buffer position and length of
      // where the headerReCopyToFrom will take place, which should be
      // where headerCopyToFrom was told to do the copy.

      int mInitialPosition;
      int mInitialLength;
   };

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Udp receive message socket.
   // Messages are based on the Ris::ByteContent message encapsulation scheme.

   class  UdpRxTMessageSocket : public Sockets::BaseUdpSocket
   {
   public:
      UdpRxTMessageSocket(); 

     //------------------------------------------------------------------------
     // Socket:

      // These do socket and bind calls
      void configure(
         char*                    aLocalIpAddr,
         int                      aLocalIpPort,
         Ris::BaseTMessageCopier* aMsgCopier);

      // This receives a message from the socket via blocking recvfrom calls.
      // It returns true if successful.
      // The recvfrom address is stored in mFromAddress.

      bool doReceiveMessage (Ris::ByteTMessage*& aMsg);

      Sockets::SocketAddress mFromAddress;

      // This is a record copier that is used to copy a record from a
      // byte buffer.It allows the doReceiveMessage method to receive and
      // extract a record from a byte buffer without the having the
      // record type visible to it.

      Ris::BaseTMessageCopier* mMsgCopier;

      //------------------------------------------------------------------------
      // State:

      // General purpose valid flag
      bool mValidFlag;

      // Metrics
      int mRxCount;
      int mRxLength;
   };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Udp transmit message socket.
   // Messages are based on the Ris::ByteContent message encapsulation scheme.

   class  UdpTxTMessageSocket : public Sockets::BaseUdpSocket
   {
   public:
      UdpTxTMessageSocket(); 

     //------------------------------------------------------------------------
     // Socket, these two should be used together

      // These create and configure the socket
      void configure(
         char*                    aRemoteIpAddr,
         int                      aRemoteIpPort,
         Ris::BaseTMessageCopier* aMsgCopier);

      // This sends a message over the socket via a blocking send call.
      // It returns true if successful.
      // It is protected by the transmit mutex.

      bool doSendMsg(Ris::ByteTMessage* aMsg);

      //------------------------------------------------------------------------
      // This is a record copier that is used to copy a record to a byte
      // buffer. It allows the doSendMsg method to send a record to a byte 
      // buffer without the having the record code visible to it.

      Ris::BaseTMessageCopier* mMsgCopier;

      //------------------------------------------------------------------------
      // Transmit mutex is used by doSendMsg for mutual exclusion.

      Ris::Threads::MutexSemaphore  mTxMutex;

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

