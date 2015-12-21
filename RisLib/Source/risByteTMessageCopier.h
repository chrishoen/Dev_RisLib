#ifndef _RISBYTETMESSAGECOPIER_H_
#define _RISBYTETMESSAGECOPIER_H_
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteTMessage.h"
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

class  BaseTMessageCopier
{
public:

   //***********************************************************************
   // This creates a new record, based on a record type

   virtual Ris::ByteTMessage* createMessage (int aType)=0;

   //***********************************************************************
   // This copies byte buffers to/from records

   virtual void copyToFrom( Ris::ByteBuffer* aBuffer, Ris::ByteTMessage* aTMessage)=0;
};

}//namespace
#endif

