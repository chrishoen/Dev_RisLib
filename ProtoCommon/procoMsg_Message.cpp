/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <stdio.h>
#include <string.h>
#include "procoMsg_Message.h"


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

void TestMsg::initialize()
{
   mCode1 = 901;
   mCode2 = 902;
   mCode3 = 903;
   mCode4 = 904;
}

void TestMsg::show()
{
   printf("%d\n",  mCode1   );
   printf("%d\n",  mCode1   );
   printf("%d\n",  mCode2   );
   printf("%d\n",  mCode3   );
   printf("\n");
}

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
   mX1 = 0;
   mX2 = 0;
   mX3 = 0;
   mX4 = 0;
} 

void DataRecord::copyToFrom (Ris::ByteBuffer* aBuffer)
{
   aBuffer->copy( &mX1 );
   aBuffer->copy( &mX2 );
   aBuffer->copy( &mX3 );
   aBuffer->copy( &mX4 );
}

void DataRecord::initialize()
{
   mX1 = 701;
   mX2 = 702;
   mX3 = 703;
   mX4 = 704;
}

void DataRecord::show()
{
   printf("%d\n",  mX1   );
   printf("%d\n",  mX2   );
   printf("%d\n",  mX3   );
   printf("%d\n",  mX4   );
   printf("\n");
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

void DataMsg::initialize()
{
   mUChar  = 0x11;
   mUShort = 0x1234;
   mUInt   = 0x12345678;
   mUInt64 = 0x1112131415161718;
   mChar   = 0x11;
   mShort  = 0x1234;
   mInt    = 0x12345678;
   mInt64  = 0x1112131415161718;
   mFloat  = 12.34f;
   mDouble = 56.78;
   mBool   = true;
   strcpy(mString1, "abcdef");
   strcpy(mString2, "01234567");

   mDataRecord.initialize();
}

void DataMsg::show()
{
   printf("UChar    %hhX\n",  mUChar   );
   printf("UShort   %hX\n",   mUShort  );
   printf("UInt     %X\n",    mUInt    );
   printf("Unit64   %llX\n",  mUInt64  );
   printf("Char     %hhX\n",  mChar    );
   printf("Short    %hX\n",   mShort   );
   printf("Int      %X\n",    mInt     );
   printf("Int64    %llX\n",  mInt64   );
   printf("Float    %5.2f\n", mFloat   );
   printf("Double   %5.2f\n", mDouble  );
   printf("Bool     %d\n",    mBool    );
   printf("String1  %s\n",    mString1 );
   printf("String2  %s\n",    mString2 );
   mDataRecord.show();
   printf("\n");
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


