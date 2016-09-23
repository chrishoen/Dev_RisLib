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
   // Return the number of bytes that need to be allocated for a message. 

   virtual int bufferSize()=0;

   //***************************************************************************
   // Create a new record, based on a record type.

   virtual Ris::ByteMsg* createMessage (int aType)=0;

   //***************************************************************************
   // Copy a record to/from a byte buffer.

   virtual void copyToFrom( Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aMsg)=0;
};

}//namespace
#endif

