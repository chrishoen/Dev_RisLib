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

//****************************************************************************
//****************************************************************************
//****************************************************************************
// MsgCopier

void MsgMonkey::copyMsgToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteMsg* aMsg)
{
   ExampleMsg::MsgCopier tCopier;
   tCopier.copyToFrom(aBuffer, aMsg);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// Copy a message to/from a byte buffer.

   // Copy into the header instance from a byte buffer.

void MsgMonkey::copyHeaderFrom(Ris::ByteBuffer* aBuffer)
{

}

}//namespace
