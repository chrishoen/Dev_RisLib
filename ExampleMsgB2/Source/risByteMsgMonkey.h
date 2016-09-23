#ifndef _RISBYTEMSGMONKEY_H_
#define _RISBYTEMSGMONKEY_H_
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteMsg.h"
#include "risByteBuffer.h"

namespace Ris
{

//******************************************************************************
// This is an abstract base class for a message parser. It can be used
// by code that receives messages into byte buffers such that the message
// classes don't have to be visible to the receiving code. Inheriting classes
// provide all of the details that are needed by receiving code to receive and
// extract messages, as opposed to having the message classes being visible
// to the receiving code.

class  BaseMsgMonkey
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constructors and initialization. 

   BaseMsgMonkey();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   //***************************************************************************
   // Create a new message, based on a record type.

   virtual Ris::ByteMsg* createMsg (int aType)=0;

   //***************************************************************************
   // Copy a message to/from a byte buffer.

   virtual void copyMsgToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aMsg)=0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy into the header instance from a byte buffer.

   virtual void copyHeaderFrom(Ris::ByteBuffer* aBuffer)=0;

   //***************************************************************************
   // Validate the header.

   virtual bool validateHeader()=0;

   //***************************************************************************
   // Access the message header identifiier.

   virtual int  getHeaderMsgId()=0;
   virtual void setHeaderMsgId(int aMsgId)=0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************

   // Return a contant max buffer size.
   virtual int bufferSize()=0;
   // Return a contant header length.
   virtual int headerLength()=0;

   // Extract message header parameters from a buffer and validate them.
   // Return true if the header is valid.
   virtual bool extractMessageHeaderParms(Ris::ByteBuffer* aBuffer) {return false;}

   // Message header parameters, these are common to all message headers.
   // They are extracted from an actual received message header. In some
   // form, all message headers contain these parameters.

   int  mHeaderLength;
   int  mMessageLength;
   int  mMessageType;
   int  mPayloadLength;
   bool mHeaderValidFlag;

   // Create a new message based on a message type.
   virtual Ris::ByteContent* createMessage(int aMessageType) {return 0;}

   // Extract a complete message from a byte buffer
   Ris::ByteContent* makeFromByteBuffer(Ris::ByteBuffer* buffer);

   // Preprocess a message before it is sent
   virtual void processBeforeSend(Ris::ByteContent* msg){};

   // Endianess for buffers associated with the parser.
   // If true then the messages will be sent in network order,
   // big endian. If false, then little endian.
   void setNetworkOrder (bool aNetworkOrder);
   bool mNetworkOrder;

   // Configures a byte buffer endianess
   void configureByteBuffer(ByteBuffer* aBuffer);
};

}//namespace
#endif

