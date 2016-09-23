#ifndef _EXAMPLEMSGMONKEY_H_
#define _EXAMPLEMSGMONKEY_H_

#include "risByteMsgMonkey.h"

namespace ExampleMsg
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MsgMonkey : public Ris::BaseMsgMonkey
{
public:

   //**************************************************************************
   // Constructor and initialization.

   MsgMonkey();

   //***************************************************************************
   // Return the number of bytes that need to be allocated for a message. 

   int bufferSize() { return 20000; }

   //***************************************************************************
   // This creates a new record, based on a record type

   Ris::ByteMsg* createMessage (int aType) override;

   //***************************************************************************
   // This copies byte buffers to/from records

   void copyToFrom( Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aMsg) override;

};
}
#endif
