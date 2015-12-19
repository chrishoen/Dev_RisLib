#ifndef _RISBYTERECORDCOPIER_H_
#define _RISBYTERECORDCOPIER_H_
//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risByteRecord.h"
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

class  BaseRecordCopier
{
public:

   //***********************************************************************
   // This creates a new record, based on a record type

   Ris::ByteRecord* (*mCreateRecord)(int aRecordTYpe);

   //***********************************************************************
   // This copies byte buffers to/from records

   void (*mCopyToFrom)(Ris::ByteBuffer* aBuffer, Ris::ByteRecord* aRecord);
};

}//namespace
#endif

