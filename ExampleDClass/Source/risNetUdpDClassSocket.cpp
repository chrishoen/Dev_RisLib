/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdlib.h>

#include "prnPrint.h"

#include "risNetUdpDClassSocket.h"

namespace Ris
{
namespace Net
{
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   DClassMsgHeader::DClassMsgHeader()
   {
      mSyncWord1         = 0x11111111;
      mSyncWord2         = 0x22222222;
      mMessageIdentifier = 0;
      mMessageLength     = 0;
      mSourceId          = 0;
      mDestinationId     = 0;

      mInitialPosition   = 0;
      mInitialLength     = 0;
      mHeaderValidFlag   = false;
   }

   void DClassMsgHeader::reset()
   {
      mSyncWord1         = 0;
      mSyncWord2         = 0;
      mMessageIdentifier = 0;
      mMessageLength     = 0;
      mSourceId          = 0;
      mDestinationId     = 0;
      mInitialPosition   = 0;
      mInitialLength     = 0;
      mHeaderValidFlag   = false;
   }


   bool DClassMsgHeader::validate()
   {
      // Test for error
      bool tError =
         mSyncWord1 != 0x11111111 ||
         mSyncWord2 != 0x22222222 ||
         mMessageLength < DClassMsgHeader::cLength  ||
         mMessageLength > DClassSocketDefT::cMsgBufferSize;

      // If no error then valid
      mHeaderValidFlag = !tError;

      // Return valid flag
      return mHeaderValidFlag;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //---------------------------------------------------------------------------
   // If the byte buffer is configured for put operations then this puts the
   // contents of the object into the byte buffer (it does a copy to, it
   // copies the object to the byte buffer).
   // If the byte buffer is configured for get operations then this gets the
   // contents of the object from the byte buffer (it does a copy from, it
   // copies the object from the byte buffer).
   // Copy To and Copy From are symmetrical.
   //---------------------------------------------------------------------------

   void DClassMsgHeader::copyToFrom (Ris::ByteBuffer* aBuffer)
   {
      aBuffer->copy( &mSyncWord1         );
      aBuffer->copy( &mSyncWord2         );
      aBuffer->copy( &mMessageIdentifier );
      aBuffer->copy( &mMessageLength     );
      aBuffer->copy( &mSourceId          );
      aBuffer->copy( &mDestinationId     );
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //---------------------------------------------------------------------------
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
   // where the copy is to take place. Both are also passed a MessageByte
   // pointer to where they can get and mMessageType
   // which they transfer into and out of the headers.
   //---------------------------------------------------------------------------

   void DClassMsgHeader::headerCopyToFrom (Ris::ByteBuffer* aBuffer,Ris::DHandle* aParent)
   {
      //------------------------------------------------------------------------
      // Instances of this class are members of parent message classes.
      // A call to this function should be the first line of code in a
      // containing parent message class's copyToFrom. It performs precopyToFrom
      // operations. It's purpose is to copy headers to/from byte buffers. The
      // corresponding function headerReCopyToFrom should be called as the last
      // line of code in the containing message class' copyToFrom. Lines of code
      // in between should copy individual data elements into/out of the buffer.

      //------------------------------------------------------------------------
      // for a "copy to" put
      //
      // If this is a "copy to" put operation then the header copy will actually
      // be done by the headerReCopyToFrom, after the rest of the message has been
      // copied into the buffer. This is because some of the fields in the header
      // cannot be set until after the rest of the message has been put into the
      // buffer. (You don't know the length of the message until you put all of
      // the data into it, you also can't compute a checksum). This call stores
      // the original buffer position that is passed to it when it is called for
      // later use by the headerReCopyToFrom. The original buffer position points
      // to where the header should be copied. This call then forward advances
      // the buffer to point past the header. Forward advancing the buffer
      // position to point just after where the header should be is the same as
      // doing a pretend copy of the header. After this pretend copy of the
      // header, the buffer position points to where the data should be put into
      // the buffer.
      //
      // Store the original buffer position for later use by the
      // headerReCopyToFrom and advance the buffer position forward
      // to point past the header.

      if (aBuffer->isCopyTo())
      {
         // Store the buffer parameters for later use by the
         // headerReCopyToFrom
         mInitialPosition = aBuffer->getPosition();
         mInitialLength   = aBuffer->getLength();

         // Advance the buffer position to point past the header.
         aBuffer->forward(cLength);
      }

      //---------------------------------------------------------------------
      // for a "copy from" get
      //
      // If this is a "copy from" get operation then copy the header from the
      // buffer into the header member. Also set the message content type from
      // the variable datum id

      else
      {
         // Copy the buffer content into the header object.
         copyToFrom(aBuffer);
         // Set the message content type.
         aParent->mType = mMessageIdentifier;
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   void DClassMsgHeader::headerReCopyToFrom  (Ris::ByteBuffer* aBuffer,Ris::DHandle* aParent)
   {
      // If this is a put operation then this actually copies the header into
      // the buffer.
      // This sets some header length parameters and copies the header into the
      // buffer position that was stored when headerCopyToFrom was called.

      if (aBuffer->isCopyTo())
      {
         // Store the buffer parameters for later use by the
         // headerReCopyToFrom
         int tFinalPosition = aBuffer->getPosition();
         int tFinalLength   = aBuffer->getLength();

         // Get message parameters from parent
         mMessageIdentifier = aParent->mType;
         mMessageLength     = aBuffer->getLength();

         // Restore buffer parameters
         // to the initial position
         aBuffer->setPosition (mInitialPosition);
         aBuffer->setLength   (mInitialPosition);

         // Copy the adjusted header into the buffer'
         // at the original position
         copyToFrom( aBuffer );

         // Restore buffer parameters
         // to the final position
         aBuffer->setPosition (tFinalPosition);
         aBuffer->setLength   (tFinalPosition);
      }
      else
      {
      }
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   UdpRxDClassSocket::UdpRxDClassSocket()
   {
      mRxBuffer = (char*)malloc(DClassSocketDefT::cMsgBufferSize);
      mRxLength     = 0;
      mRxCount      = 0;
      mValidFlag    = false;
      mDClassCopier = 0;
   }

   //***************************************************************************

   UdpRxDClassSocket::~UdpRxDClassSocket()
   {
      free(mRxBuffer);
   }

   //***************************************************************************
   // Configure the socket

   void UdpRxDClassSocket::configure(
      char*             aLocalIpAddr,
      int               aLocalIpPort,
      BaseDClassCopier* aClassCopier)
   {
      mRxCount=0;

      mLocal.set(aLocalIpAddr,aLocalIpPort);
      mDClassCopier = aClassCopier;

      doSocket();
      doBind();

      if (mStatus==0)
      {
         Prn::print(Prn::SocketInit2, "UdpRxDClassSocket     $ %16s : %d",
            mLocal.mIpAddr.mString,
            mLocal.mPort);
      }
      else
      {
         Prn::print(Prn::SocketInit2, "UdpRxDClassSocket     $ %16s : %d $ %d %d",
            mLocal.mIpAddr.mString,
            mLocal.mPort,
            mStatus,
            mError);
      }

      mValidFlag=mStatus==0;
   }

   //***************************************************************************
   // This receives a datagram from the socket into a byte buffer and then
   // extracts a message from the byte buffer

   bool UdpRxDClassSocket::doRecvDClass (DHandle& aClass)
   {
      //------------------------------------------------------------------------
      // Initialize
      aClass.reset();

      // Guard
      if (!mValidFlag) return false;

      // Byte buffer, constructor takes address and size
      ByteBuffer tBuffer(mRxBuffer,DClassSocketDefT::cMsgBufferSize);  
      tBuffer.setCopyFrom();

      //------------------------------------------------------------------------
      // Read the message into the receive buffer
   
      doRecvFrom  (mFromAddress,mRxBuffer,mRxLength,DClassSocketDefT::cMsgBufferSize);

      // Guard
      // If bad status then return false.
      // Returning true  means socket was not closed
      // Returning false means socket was closed

      if (mRxLength<=0)
      {
         if (mStatus<0)
            switch (mError)
            {
               case 0              : return false  ;break;
               default             : return false  ;break;
            }   
         else
            return false;
      }

      // Set the buffer length
      tBuffer.setLength(mRxLength);
   
      //--------------------------------------------------------------
      // Copy from the receive buffer into an instance of the header
      // and validate the header

      DClassMsgHeader tHeader;

      tBuffer.setCopyFrom();
      tBuffer.getFromBuffer(&tHeader);

      // If the header is not valid then error
      
      if (!tHeader.validate())
      {
         Prn::print(Prn::SocketRun1, "ERROR doRecv1 INVALID HEADER ");
         return false;
      }

      //--------------------------------------------------------------
      // At this point the buffer contains the complete message.
      // Extract the message from the byte buffer into a new message
      // object and return it.

      // Create a record based on the record type
      aClass = mDClassCopier->createDClass(tHeader.mMessageIdentifier);

      // Copy from the buffer into the record
      mDClassCopier->copyToFrom(&tBuffer, aClass);

      // Test for errors and return.
      // If the pointer is zero then message is bad
      if (aClass.mPtr==0)
      {
         mStatus=tBuffer.getError();
      }

      // Returning true  means socket was not closed
      // Returning false means socket was closed
      mRxCount++;
      return true;
   }

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   UdpTxDClassSocket::UdpTxDClassSocket()
   {
      mTxCount      = 0;
      mTxLength     = 0;
      mValidFlag    = false;
      mDClassCopier = 0;
   }

   //***************************************************************************
   // Configure the socket. Use with the next doSendMsg.

   void UdpTxDClassSocket::configure(
      char*             aRemoteIpAddr,
      int               aRemoteIpPort,
      BaseDClassCopier* aClassCopier)
   {
      mTxCount=0;

      mRemote.set(aRemoteIpAddr,aRemoteIpPort);
      mDClassCopier = aClassCopier;

      doSocket();

      if (mStatus==0)
      {
         Prn::print(Prn::SocketInit2, "UdpTxDClassSocket     $ %16s : %d",
            mRemote.mIpAddr.mString,
            mRemote.mPort);
      }
      else
      {
         Prn::print(Prn::SocketInit2, "UdpTxDClassSocket     $ %16s : %d $ %d %d",
            mRemote.mIpAddr.mString,
            mRemote.mPort,
            mStatus,
            mError);
      }

      mValidFlag=mStatus==0;
   }

   //***************************************************************************
   // This copies a message into a byte buffer and then sends the byte buffer 
   // out the socket. Use with the previous configure.

   bool UdpTxDClassSocket::doSendDClass(DHandle aClass)
   {
      // Guard
      if (!mValidFlag) return false;

      // Create byte buffer, constructor takes size
      ByteBuffer tBuffer(DClassSocketDefT::cMsgBufferSize);

      //------------------------------------------------------------------------
      // Instance of a header,set members

      DClassMsgHeader tHeader;
      tHeader.mMessageIdentifier = aClass.mType;

      //------------------------------------------------------------------------
      // Copy

      // Copy header to buffer
      tBuffer.setCopyTo();
      tHeader.headerCopyToFrom(&tBuffer,&aClass);

      // Copy record to buffer
      mDClassCopier->copyToFrom(&tBuffer,aClass);
      
      // ReCopy header to buffer
      tHeader.headerReCopyToFrom(&tBuffer,&aClass);

      // Delete the record
      aClass.deallocate();

      //------------------------------------------------------------------------
      // Send buffer to socket

      // Mutex
      mTxMutex.get();

      // Transmit the buffer
      mTxLength=tBuffer.getLength();
      doSendTo(mRemote,tBuffer.getBaseAddress(),mTxLength);
      Prn::print(Prn::SocketRun1, "UdpTxDClassSocket  doSendTo   $ %d",mTxLength);

      mTxCount++;

      // Mutex
      mTxMutex.put();

      return true;
   }


}//namespace
}//namespace

