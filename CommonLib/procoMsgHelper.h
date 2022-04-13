#pragma once

#include "procoMsg.h"

namespace ProtoComm
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

class MsgHelper
{
public:

   static void initialize(TestMsg* aMsg);
   static void initialize(FirstMessageMsg* aMsg);
   static void initialize(EchoRequestMsg* aMsg,int aNumWords = 0);
   static void initialize(EchoResponseMsg* aMsg, int aNumWords = 0);
   static void initialize (DataRecord* aMsg);
   static void initialize(DataMsg* aMsg);
   static void initialize(ByteBlobMsg* aMsg);
   static void initialize2(ByteBlobMsg* aMsg);

   static void show(int aPF, TestMsg* aMsg);
   static void show(int aPF, FirstMessageMsg* aMsg);
   static void show(int aPF, EchoRequestMsg* aMsg);
   static void show(int aPF, EchoResponseMsg* aMsg);
   static void show(int aPF, DataRecord* aMsg);
   static void show(int aPF, DataMsg* aMsg);
   static void show(int aPF, ByteBlobMsg* aMsg);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
