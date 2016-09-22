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
   // This creates a new record, based on a record type

   Ris::ByteMsgB* createMessage (int aType) override;

   //***************************************************************************
   // This copies byte buffers to/from records

   void copyToFrom( Ris::ByteBuffer* aBuffer, Ris::ByteMsgB* aMsgB) override;

};
}
#endif
