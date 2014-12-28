#ifndef _RISBYTEMESSAGE_H_
#define _RISBYTEMESSAGE_H_
/*==============================================================================
ByteContent support classes for messages.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risPortableTypes.h"
#include "risByteContent.h"

namespace Ris
{

//******************************************************************************
// This class contains general parameters that describe message characteristics.

class  MessageHeaderParms
{
public:
   MessageHeaderParms();

   int  mHeaderLength;
   int  mMessageLength;
   int  mMessageType;
   int  mPayloadLength;

   bool mValidFlag;
   bool isValid(){return mValidFlag;}
};

//******************************************************************************
// This is an abstract base class for a message parser. It can be used
// by code that receives messages into byte buffers such that the message
// classes don't have to be visible to the receiving code. Inheriting classes
// provide all of the details that are needed by receiving code to receive and
// extract messages, as opposed to having the message classes being visible
// to the receiving code.

class  BaseMessageParser
{
public:
   BaseMessageParser();

   // Return a contant header length
   virtual int getHeaderLength()=0;

   // Extract message header parameters from a buffer and validate them
   virtual void getMessageHeaderParms(Ris::ByteBuffer* aBuffer,MessageHeaderParms* aParms)=0;

   // create a new message based on a message type
   virtual Ris::ByteContent* createMessage(int aMessageType)=0;

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

//******************************************************************************
// This is an abstract base class for a message parser creator. It defines
// a method that inheriting classes overload to create new message parsers.
// It is used by transmitters and receivers to create new instances of message
// parsers.

class  BaseMessageParserCreator
{
public:
   virtual BaseMessageParser* createNew() = 0;
};

//******************************************************************************
// This is a base class for messages that use ByteContent. It supplies some
// member variables that are used for message identification. 

class  MessageContent : public ByteContent
{
public:
   MessageContent()
   {
      mFamily=0;
      mMessageType=0;
   }
   
   // Message Type Identifiers
   int mFamily;
   int mMessageType;
};

//******************************************************************************
// This is a base class for files that use ByteContent. It supplies some
// member variables that are used for file identification. 

class  FileContent : public ByteContent
{
public:
   FileContent()
   {
      mFileType=0;
   }
   
   // Identifiers
   int mFileType;
};

}//namespace
#endif

