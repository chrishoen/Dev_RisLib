/*==============================================================================
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "procoMsg.h"


namespace ProtoComm
{
//******************************************************************************
//******************************************************************************
//******************************************************************************

TestMsg::TestMsg()
{
   mMessageType = MsgIdT::cTestMsg;
   mCode1 = 901;
   mCode2 = 902;
   mCode3 = 903;
   mCode4 = 904;
} 

void TestMsg::show(int aPF)
{
   Prn::print(aPF, "TestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "Code2      %d", mCode2);
   Prn::print(aPF, "Code3      %d", mCode3);
   Prn::print(aPF, "Code4      %d", mCode4);
   Prn::print(aPF, "");
}

void TestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
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

FirstMessageMsg::FirstMessageMsg()
{
   mMessageType = MsgIdT::cFirstMessageMsg;
   mCode1 = 0;
} 

void FirstMessageMsg::show(int aPF)
{
   Prn::print(aPF, "FirstMessageMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "");
}

void FirstMessageMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy( & mCode1 );

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

EchoRequestMsg::EchoRequestMsg()
{
   mMessageType = MsgIdT::cEchoRequestMsg;

   mCode1 = 101;
   mCode2 = 102;
   mCode3 = 103;
   mCode4 = 104;

   mNumWords = 0;
   mWords[0] = 0;
} 

void EchoRequestMsg::initialize(int aNumWords)
{
   mCode1 = 301;
   mCode2 = 302;
   mCode3 = 303;
   mCode4 = 304;
   mNumWords = aNumWords;
}

void EchoRequestMsg::show(int aPF)
{
   Prn::print(aPF, "EchoRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "NumWords   %d", mNumWords);
   Prn::print(aPF, "");
}

void EchoRequestMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer,this);

   aBuffer->copy        (& mCode1    );
   aBuffer->copy        (& mCode2    );
   aBuffer->copy        (& mCode3    );
   aBuffer->copy        (& mCode4    );

   aBuffer->copy        (& mNumWords );
   for (int i=0;i<mNumWords;i++)
   {
      aBuffer->copy     (& mWords[i] );
   }

   mHeader.headerReCopyToFrom(aBuffer,this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

EchoResponseMsg::EchoResponseMsg()
{
   mMessageType = MsgIdT::cEchoResponseMsg;

   mCode1 = 201;
   mCode2 = 202;
   mCode3 = 203;
   mCode4 = 204;

   mNumWords = MaxWords;
   mWords[0] = 0;
}

void EchoResponseMsg::initialize(int aNumWords)
{
   mCode1 = 301;
   mCode2 = 302;
   mCode3 = 303;
   mCode4 = 304;
   mNumWords = aNumWords;
}

void EchoResponseMsg::show(int aPF)
{
   Prn::print(aPF, "EchoRequestMsg");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "NumWords   %d", mNumWords);
   Prn::print(aPF, "");
}

void EchoResponseMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   aBuffer->copy(&mCode1);
   aBuffer->copy(&mCode2);
   aBuffer->copy(&mCode3);
   aBuffer->copy(&mCode4);

   aBuffer->copy(&mNumWords);
   for (int i = 0; i<mNumWords; i++)
   {
      aBuffer->copy(&mWords[i]);
   }

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

DataRecord::DataRecord()
{
   mCode1 = 0;
   mCode2 = 0;
   mCode3 = 0;
   mCode4 = 0;
} 

void DataRecord::show(int aPF)
{
   Prn::print(aPF, "DataRecord");
   Prn::print(aPF, "Code1      %d", mCode1);
   Prn::print(aPF, "Code2      %d", mCode2);
   Prn::print(aPF, "Code3      %d", mCode3);
   Prn::print(aPF, "Code4      %d", mCode4);
   Prn::print(aPF, "");
}

void DataRecord::copyToFrom(Ris::ByteBuffer* aBuffer)
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

void DataMsg::initialize()
{
   mUChar = 0xA1;
   mUShort = 0xBBB2;
   mUInt = 0xCCCCCCC3;
   mUInt64 = 0xDDDDDDD4EEEEEEE5;
   mChar = 101;
   mShort = 10001;
   mInt = 1000000001;
   mInt64 = 10000000000001;
   mFloat = 1.2345f;
   mDouble = 2.34567;
   mBool = false;

   strcpy(mString1, "ABCDEFGH");
   strcpy(mString2, "abcdefgh");
}

void DataMsg::show(int aPF)
{
   Prn::print(aPF, "DataMsg");
   Prn::print(aPF, "UShort     %X", mUShort);
   Prn::print(aPF, "UInt       %X", mUInt);
   Prn::print(aPF, "UInt64     %llX", mUInt64);
   Prn::print(aPF, "Char       %d", mChar);
   Prn::print(aPF, "Short      %d", mShort);
   Prn::print(aPF, "Int        %d", mInt);
   Prn::print(aPF, "Int64      %lld", mInt64);
   Prn::print(aPF, "Float      %f", mFloat);
   Prn::print(aPF, "Double     %f", mDouble);
   Prn::print(aPF, "Bool       %d", mBool);

   Prn::print(aPF, "String1    %s", mString1);
   Prn::print(aPF, "String2    %s", mString2);

   mDataRecord.show(aPF);
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
   aBuffer->copyZ( mString2, cMaxStringSize);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

ByteBlobMsg::ByteBlobMsg()
{
   mMessageType = MsgIdT::cByteBlobMsg;
}

void ByteBlobMsg::initialize()
{
   int tInt = 0x10111213;
   mBytes.mCopySize = 4;
   memcpy(mBytes.mBytes, &tInt, 4);
}

void ByteBlobMsg::initialize2()
{
   mBytes.mCopySize = 1000;
   memset(mBytes.mBytes, 0x77, 1000);
}

void ByteBlobMsg::show(int aPF)
{
   int tInt = 0;
   memcpy(&tInt, mBytes.mBytes, 4);

   Prn::print(aPF, "ByteBlobMsg");
   Prn::print(aPF, "CopySize   %d", mBytes.mCopySize);
   Prn::print(aPF, "Bytes      %08x", tInt);
}

void ByteBlobMsg::copyToFrom(Ris::ByteBuffer* aBuffer)
{
   mHeader.headerCopyToFrom(aBuffer, this);

   // If copy to   then the copy size is set beforehand.
   // If copy from then the copy size is the entire payload size.
   if (aBuffer->isCopyFrom())
   {
      mBytes.mCopySize = mHeader.mMessageLength - Header::cLength;
   }
   aBuffer->copy(&mBytes);

   mHeader.headerReCopyToFrom(aBuffer, this);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a new message, based on a message type.

void* createMsg(int aMessageType)
{
   Ris::ByteContent* tMsg = 0;

   switch (aMessageType)
   {
   case MsgIdT::cTestMsg:
      tMsg = new TestMsg;
      break;
   case MsgIdT::cFirstMessageMsg:
      tMsg = new FirstMessageMsg;
      break;
   case MsgIdT::cEchoRequestMsg:
      tMsg = new EchoRequestMsg;
      break;
   case MsgIdT::cEchoResponseMsg:
      tMsg = new EchoResponseMsg;
      break;
   case MsgIdT::cDataMsg:
      tMsg = new DataMsg;
      break;
   case MsgIdT::cByteBlobMsg:
      tMsg = new ByteBlobMsg;
      break;
   default:
      return 0;
      break;
   }
   return tMsg;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


