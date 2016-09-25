/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include "procoMsg.h"


namespace ProtoComm
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

TestMsg::TestMsg ()
{
   mMessageType = MsgIdT::cTestMsg;
   mCode1 = 901;
   mCode2 = 902;
   mCode3 = 903;
   mCode4 = 904;
} 

void TestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( &mCode1 );
   aBuffer->copy( &mCode2 );
   aBuffer->copy( &mCode3 );
   aBuffer->copy( &mCode4 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

FirstMessageMsg::FirstMessageMsg ()
{
   mMessageType = MsgIdT::cFirstMessageMsg;
   mCode1 = 0;
} 

void FirstMessageMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( & mCode1 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

StatusRequestMsg::StatusRequestMsg ()
{
   mMessageType = MsgIdT::cStatusRequestMsg;

   mCode1 = 101;
   mCode2 = 102;
   mCode3 = 103;
   mCode4 = 104;

   mNumOfWords=0;
   mNumOfWords=MaxWords;
} 

void StatusRequestMsg::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy        (& mCode1  );
   aBuffer->copy        (& mCode2  );
   aBuffer->copy        (& mCode3  );
   aBuffer->copy        (& mCode4  );

   aBuffer->copy        (& mNumOfWords  );
   for (int i=0;i<mNumOfWords;i++)
   {
      aBuffer->copy     (& mWords[i] );
   }

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

StatusResponseMsg::StatusResponseMsg()
{
   mMessageType = MsgIdT::cStatusResponseMsg;

   mCode1 = 201;
   mCode2 = 202;
   mCode3 = 203;
   mCode4 = 204;

   mNumOfWords = 0;
   mNumOfWords = MaxWords;
}

void StatusResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);
   aBuffer->copy(&mCode2);
   aBuffer->copy(&mCode3);
   aBuffer->copy(&mCode4);

   aBuffer->copy(&mNumOfWords);
   for (int i = 0; i<mNumOfWords; i++)
   {
      aBuffer->copy(&mWords[i]);
   }

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

DataRecord::DataRecord ()
{
   mCode1 = 0;
   mCode2 = 0;
   mCode3 = 0;
   mCode4 = 0;
} 

void DataRecord::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   aBuffer->copy( &mCode1 );
   aBuffer->copy( &mCode2 );
   aBuffer->copy( &mCode3 );
   aBuffer->copy( &mCode4 );
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

DataMsg::DataMsg()
{
   mMessageType = MsgIdT::cDataMsg;

   mUChar  = 0;
   mUShort = 0;
   mUInt   = 0;
   mUInt64 = 0;
   mChar   = 0;
   mShort  = 0;
   mInt    = 0;
   mInt64  = 0;
   mFloat  = 0.0f;
   mDouble = 0.0;
   mBool   = false;

   mString1[0]=0;
   mString2[0]=0;
}

void DataMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy( &mUChar  );
   aBuffer->copy( &mUShort );
   aBuffer->copy( &mUInt   );
   aBuffer->copy( &mUInt64 );
   aBuffer->copy( &mChar   );
   aBuffer->copy( &mShort  );
   aBuffer->copy( &mInt    );
   aBuffer->copy( &mInt64  );
   aBuffer->copy( &mFloat  );
   aBuffer->copy( &mDouble );
   aBuffer->copy( &mBool   );
   aBuffer->copy( &mDataRecord );
   aBuffer->copyS( mString1 );
   aBuffer->copyS( mString2 );

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This creates a new message, based on a message type

Ris::ByteContent* MsgCreator::createMsg (int aMessageType)
{
   Ris::ByteContent* tMsg = 0;

   switch (aMessageType)
   {
   case MsgIdT::cTestMsg :
      tMsg = new TestMsg;
      break;
   case MsgIdT::cFirstMessageMsg :
      tMsg = new FirstMessageMsg;
      break;
   case MsgIdT::cStatusRequestMsg :
      tMsg = new StatusRequestMsg;
      break;
   case MsgIdT::cStatusResponseMsg :
      tMsg = new StatusResponseMsg;
      break;
   case MsgIdT::cDataMsg :
      tMsg = new DataMsg;
      break;
   default :
      return 0;
      break;
   }
   return tMsg;
}

}//namespace


