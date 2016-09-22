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
// This creates a new record, based on a record type

Ris::ByteMsgB* MsgMonkey::createMessage(int aMsgType)
{
   ExampleMsg::MsgCreator tCreator;
   return tCreator.createMessage(aMsgType);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
// MsgBCopier

void MsgMonkey::copyToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteMsgB* aMsg)
{
   ExampleMsg::MsgCopier tCopier;
   tCopier.copyToFrom(aBuffer, aMsg);
}

}//namespace
