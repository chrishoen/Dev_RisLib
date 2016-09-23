#include <stdio.h>
#include <string.h>

#include "exampleMsg.h"
#include "exampleMsgMonkey.h"

namespace ExampleMsg
{
//******************************************************************************
//******************************************************************************
//******************************************************************************
// Constructors and initialization.

MsgMonkey::MsgMonkey()
{
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a new message, based on a record type.

Ris::ByteMsg* MsgMonkey::createMsg(int aMsgType)
{
   ExampleMsg::MsgCreator tCreator;
   return tCreator.createMsg(aMsgType);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// MsgCopier

void MsgMonkey::copyMsgToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aMsg)
{
   ExampleMsg::MsgCopier tCopier;
   tCopier.copyToFrom(aBuffer, aMsg);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Copy into the header instance from a byte buffer.

void MsgMonkey::copyHeaderFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.copyToFrom(aBuffer);
}

//******************************************************************************
// Validate the header.

bool MsgMonkey::validateHeader()
{
   return mHeader.validate();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Access the message identifiier.

int MsgMonkey::getHeaderMsgId()
{
   return mHeader.mMessageIdentifier;
}
void MsgMonkey::setHeaderMsgId(int aMsgId)
{
   mHeader.mMessageIdentifier = aMsgId;
}


}//namespace
