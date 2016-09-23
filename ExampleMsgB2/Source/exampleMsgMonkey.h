#ifndef _EXAMPLEMSGMONKEY_H_
#define _EXAMPLEMSGMONKEY_H_

#include "risByteMsgMonkey.h"
#include "risByteMsgDefault.h"

namespace ExampleMsg
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MsgMonkey : public Ris::BaseMsgMonkey
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members. 

   // Instance of header to be used for message transmit and receive.
   Ris::DefaultMsgHeader mHeader;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constructor and initialization. 

   MsgMonkey();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Return configuration constants. 

   // Return a contant max buffer size.
   int bufferSize() override { return 20000; }
   // Return a contant header length.
   int headerLength() override { return 24; }



   //***************************************************************************
   // Create a new message, based on a record type.

   Ris::ByteMsg* createMsg (int aType) override;

   //***************************************************************************
   // Copy a message to/from a byte buffer.

   void copyMsgToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aMsg) override;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Copy into the header instance from a byte buffer.

   void copyHeaderFrom(Ris::ByteBuffer* aBuffer) override;

   //***************************************************************************
   // Validate the header.

   bool validateHeader() override;

   //***************************************************************************
   // Access the message identifiier.

   int  getHeaderMsgId() override;
   void setHeaderMsgId(int aMsgId) override;

};
}
#endif
