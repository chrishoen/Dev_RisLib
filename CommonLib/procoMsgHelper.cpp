#include "stdafx.h"

#include "procoMsgHelper.h"

namespace ProtoComm
{
//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(TestMsg* aMsg)
{
   aMsg->mCode1 = 101;
   aMsg->mCode2 = 102;
   aMsg->mCode3 = 103;
   aMsg->mCode4 = 104;
}

void MsgHelper::show(int aPF, TestMsg* aMsg)
{
   Prn::print(aPF, "TestMsg");
   Prn::print(aPF, "Code1      %d", aMsg->mCode1);
   Prn::print(aPF, "Code2      %d", aMsg->mCode2);
   Prn::print(aPF, "Code3      %d", aMsg->mCode3);
   Prn::print(aPF, "Code4      %d", aMsg->mCode4);
   Prn::print(aPF, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(FirstMessageMsg* aMsg)
{
   aMsg->mCode1 = 201;
}

void MsgHelper::show(int aPF, FirstMessageMsg* aMsg)
{
   Prn::print(aPF, "FirstMsg");
   Prn::print(aPF, "Code1      %d", aMsg->mCode1);
   Prn::print(aPF, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(EchoRequestMsg* aMsg,int aNumWords)
{
   aMsg->mCode1 = 301;
   aMsg->mCode2 = 302;
   aMsg->mCode3 = 303;
   aMsg->mCode4 = 304;
   aMsg->mNumWords = aNumWords;
}

void MsgHelper::show(int aPF, EchoRequestMsg* aMsg)
{
   Prn::print(aPF, "EchoRequestMsg");
   Prn::print(aPF, "Code1      %d", aMsg->mCode1);
   Prn::print(aPF, "NumWords   %d", aMsg->mNumWords);
   Prn::print(aPF, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(EchoResponseMsg* aMsg, int aNumWords)
{
   aMsg->mCode1 = 401;
   aMsg->mCode2 = 402;
   aMsg->mCode3 = 403;
   aMsg->mCode4 = 404;
}

void MsgHelper::show(int aPF, EchoResponseMsg* aMsg)
{
   Prn::print(aPF, "EchoResponseMsg");
   Prn::print(aPF, "Code1      %d", aMsg->mCode1);
   Prn::print(aPF, "NumWords   %d", aMsg->mNumWords);
   Prn::print(aPF, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(DataRecord* aMsg)
{
   aMsg->mCode1 = 501;
   aMsg->mCode2 = 502;
   aMsg->mCode3 = 503;
   aMsg->mCode4 = 504;
}

void MsgHelper::show(int aPF, DataRecord* aMsg)
{
   Prn::print(aPF, "DataRecord");
   Prn::print(aPF, "Code1      %d", aMsg->mCode1);
   Prn::print(aPF, "Code2      %d", aMsg->mCode2);
   Prn::print(aPF, "Code3      %d", aMsg->mCode3);
   Prn::print(aPF, "Code4      %d", aMsg->mCode4);
   Prn::print(aPF, "");
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(DataMsg* aMsg)
{
   aMsg->mUChar  = 0xA1;
   aMsg->mUShort = 0xBBB2;
   aMsg->mUInt   = 0xCCCCCCC3;
   aMsg->mUInt64 = 0xDDDDDDD4EEEEEEE5;
   aMsg->mChar   = 101;
   aMsg->mShort  = 10001;
   aMsg->mInt    = 1000000001;
   aMsg->mInt64  = 10000000000001;
   aMsg->mFloat  = 1.2345f;
   aMsg->mDouble = 2.34567;
   aMsg->mBool   = false;


   strcpy(aMsg->mString1,"ABCDEFGH");
   strcpy(aMsg->mString2,"abcdefgh");

   initialize(&aMsg->mDataRecord);
}

void MsgHelper::show(int aPF, DataMsg* aMsg)
{
   Prn::print(aPF, "DataMsg");
   Prn::print(aPF, "UShort     %X",    aMsg->mUShort);
   Prn::print(aPF, "UInt       %X",    aMsg->mUInt);
   Prn::print(aPF, "UInt64     %llX",  aMsg->mUInt64);
   Prn::print(aPF, "Char       %d",    aMsg->mChar);
   Prn::print(aPF, "Short      %d",    aMsg->mShort);
   Prn::print(aPF, "Int        %d",    aMsg->mInt);
   Prn::print(aPF, "Int64      %lld",  aMsg->mInt64);
   Prn::print(aPF, "Float      %f",    aMsg->mFloat);
   Prn::print(aPF, "Double     %f",    aMsg->mDouble);
   Prn::print(aPF, "Bool       %d",    aMsg->mBool);

   Prn::print(aPF, "String1    %s",    aMsg->mString1);
   Prn::print(aPF, "String2    %s",    aMsg->mString2);

   show(aPF, &aMsg->mDataRecord);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************

void MsgHelper::initialize(ByteBlobMsg* aMsg)
{
   int tInt = 0x10111213;
   aMsg->mBytes.mCopySize = 4;
   memcpy(aMsg->mBytes.mBytes, &tInt, 4);
}

void MsgHelper::initialize2(ByteBlobMsg* aMsg)
{
   aMsg->mBytes.mCopySize = 1000;
   memset(aMsg->mBytes.mBytes, 0x77, 1000);
}

void MsgHelper::show(int aPF, ByteBlobMsg* aMsg)
{
   int tInt = 0;
   memcpy(&tInt, aMsg->mBytes.mBytes, 4);

   Prn::print(aPF, "ByteBlobMsg");
   Prn::print(aPF, "CopySize   %d", aMsg->mBytes.mCopySize);
   Prn::print(aPF, "Bytes      %08x", tInt);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
}//namespace
