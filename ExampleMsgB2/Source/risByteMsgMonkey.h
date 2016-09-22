#ifndef _RISBYTEMSGBMONKEY_H_
#define _RISBYTEMSGBMONKEY_H_
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteMsgB.h"
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
   // Create a new record, based on a record type.

   virtual Ris::ByteMsgB* createMessage (int aType)=0;

   //***************************************************************************
   // Copy a record to/from a byte buffer.

   virtual void copyToFrom( Ris::ByteBuffer* aBuffer, Ris::ByteMsgB* aMsgB)=0;
};

}//namespace
#endif

