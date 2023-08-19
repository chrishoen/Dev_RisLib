#pragma once
/*==============================================================================
ByteContent message monkey.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteContent.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// The following rules are for shared memory, regions that are shared between
//  different processes(who therefore have different address spaces) :
//
//  1) No constructors.
//  2) No pointers.
//  3) No dynamic memory, this means no std::vector, ...
//  4) No vtables, this means no virtual functions.
//  5) Be careful with your loadsand stores.
//
// This is a base class for message metrics. It is shared memory safe.

class BaseMsgMetrics
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Message metrics.
   int mMsgCount;
   long long mByteCount;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // No constructor.
   void resetBaseVars();

   // Update the metrics with a message and a length.
   void updateBaseVars(ByteContent* aMsg, int aMsgLength);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is an abstract base class for a message monkey. It can be used
// by code that receives messages into byte buffers such that the message
// classes don't have to be visible to the receiving code. Inheriting classes
// provide all of the details that are needed by receiving code to receive and
// extract messages, as opposed to having the message classes being visible
// to the receiving code. Likewise for the transmitting code.

class BaseMsgMonkey
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Message header parameters, these are common to all message headers.
   // They are extracted from an actual received message header. In some
   // form, all message headers contain these parameters. These are extracted
   // from a byte buffer.
   int  mHeaderLength;
   int  mMessageLength;
   int  mMessageType;
   int  mPayloadLength;
   bool mHeaderValidFlag;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Message metrics. These point to the default metrics or to an
   // inheritors metrics, depending on the constructor.
   BaseMsgMetrics* mBaseTxMsgMetrics;
   BaseMsgMetrics* mBaseRxMsgMetrics;

   // Default message metrics.
   BaseMsgMetrics mDefaultTxMsgMetrics;
   BaseMsgMetrics mDefaultRxMsgMetrics;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Constructor.
   BaseMsgMonkey();
   BaseMsgMonkey(
      BaseMsgMetrics* aTxMsgMetrics,
      BaseMsgMetrics* aRxMsgMetrics);
   virtual ~BaseMsgMonkey() {}

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, header processing.

   // Extract message header parameters from a buffer and validate them
   // Returns true if the header is valid
   virtual bool extractMessageHeaderParms(Ris::ByteBuffer* aBuffer) = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, message processing.

   // Preprocess a message before it is sent.
   virtual void processBeforeSend(Ris::ByteContent* aMsg){};

   // Postprocess a message after it is sent.
   virtual void processAfterSend(Ris::ByteBuffer* aBuffer, Ris::ByteContent* aMsg) {};

   // Postprocess a message after it is received.
   virtual void processAfterReceive(Ris::ByteBuffer* aBuffer, Ris::ByteContent* aMsg) {};

   // Copy a message to a byte buffer.
   void putMsgToBuffer (Ris::ByteBuffer* aBuffer,Ris::ByteContent* aMsg);

   // Copy a message from a byte buffer.
   // The header parms must be extracted prior to calling this.
   Ris::ByteContent* getMsgFromBuffer (Ris::ByteBuffer* aBuffer);

   // Copy a message from a byte buffer.
   // This first extracts the header parms.
   Ris::ByteContent* makeMsgFromBuffer (Ris::ByteBuffer* aBuffer);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, buffer management.

   // Return a contant header length.
   virtual int getHeaderLength() = 0;

   // Return a contant max buffer size.
   virtual int getMaxBufferSize() = 0;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, footer processing.

   // Validate a message footer by comparing the contents of the byte buffer
   // that the message was received into and the message itself. For example,
   // the byte buffer can contain the message bytes and the message can contain
   // a checksum and this function would calculate the checksum on the buffer
   // bytes and then compare it with the checksum stored in the message footer.
   virtual bool validateMessageFooter(Ris::ByteBuffer* aBuffer,Ris::ByteContent* aMsg)
   { return true; };

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, metrics.

   // Reset the metrics.
   virtual void resetMsgMetrics();

   // Update the metrics with a message and a length.
   virtual void updateTxMsgMetrics(ByteContent* aMsg, int aMsgLength);
   virtual void updateRxMsgMetrics(ByteContent* aMsg, int aMsgLength);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods, create messages.

   // Create a message from the message set, base on the message type.
   virtual ByteContent* createMsg(int aMessageType) = 0;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

