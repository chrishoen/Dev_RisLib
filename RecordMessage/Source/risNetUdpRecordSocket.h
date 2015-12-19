#ifndef _RISNETUDPRECORDSOCKET_H_
#define _RISNETUDPRECORDSOCKET_H_

/*==============================================================================

UdpRxRecordSocket -- udp receive socket
UdpTxRecordSocket -- udp transmit socket

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteRecord.h"
#include "risByteContent.h"
#include "risByteRecordCopier.h"
#include "risSockets.h"
#include "risThreads.h"

namespace Ris
{
namespace Net
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Message Definitions

   namespace RecordSocketDefT
   {
      //************************************************************************
      // Use this for a buffer size for these messages

      static const int cMsgBufferSize = 20000;

   }//namespace
   
    //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // This encapsualtes the message header.

   class Header : public Ris::ByteContent
   {
   public:
      // Constructor
      Header();
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
      // where the copy is to take place. Both are also passed a ByteContent
      // pointer to where they can get and mMessageType
      // which they transfer into and out of the headers.
      //------------------------------------------------------------------------

      void headerCopyToFrom   (Ris::ByteBuffer* aBuffer, Ris::ByteRecord* aParent);
      void headerReCopyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteRecord* aParent);

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
   // Messages are based on the ByteContent message encapsulation scheme.

   class  UdpRxRecordSocket : public Sockets::BaseUdpSocket
   {
   public:
      UdpRxRecordSocket(); 
     ~UdpRxRecordSocket(); 

      //--------------------------------------------------------------
      // Socket:

      // These do socket and bind calls
      void configure(
         Sockets::SocketAddress    aLocal,
         BaseRecordCopier* aRecordCopierCreator);

      // This receives a message from the socket via blocking recvfrom calls.
      // It returns true if successful.
      // The recvfrom address is stored in mFromAddress.
      bool doRecvMsg (ByteRecord*& aRxMsg);

      Sockets::SocketAddress mFromAddress;

      // This is a message parser that is used to get details about 
      // a message from a message header that is contained in a
      // byte buffer. It allows the doRecvMsg method to receive and extract a
      // message from a byte buffer without the having the message code
      // visible to it.
      BaseRecordCopier* mRecordCopier;

      // Buffers
      char*                mRxBuffer;
      int                  mRxLength;

      //--------------------------------------------------------------
      // State:

      // General purpose valid flag
      bool mValidFlag;

      // Metrics
      int mRxCount;
   };

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Udp transmit message socket.
   // Messages are based on the ByteContent message encapsulation scheme.

   class  UdpTxRecordSocket : public Sockets::BaseUdpSocket
   {
   public:
      UdpTxRecordSocket(); 
     ~UdpTxRecordSocket(); 

      //--------------------------------------------------------------
      // Socket, these two should be used together

      // These create and configure the socket
      void configure(
         Sockets::SocketAddress      aRemote,
         BaseRecordCopier*   aRecordCopier);

      // This sends a message over the socket via a blocking send call.
      // It returns true if successful.
      // It is protected by the transmit mutex.
      bool doSendMsg(
         ByteRecord*  aTxMsg);

      //--------------------------------------------------------------

      // This is a message parser that is used to get details about 
      // a message from a message header that is contained in a
      // byte buffer. It allows the doRecvMsg method to receive and extract a
      // message from a byte buffer without the having the message code
      // visible to it.
      BaseRecordCopier* mRecordCopier;

      // Buffers
      char*                mTxBuffer;
      int                  mTxLength;

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

