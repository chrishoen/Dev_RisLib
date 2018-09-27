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

   static void initialize (TestMsg* aMsg);
   static void initialize (FirstMessageMsg* aMsg);
   static void initialize (StatusRequestMsg* aMsg);
   static void initialize (StatusResponseMsg* aMsg);

   static void initialize (DataRecord* aMsg);
   static void initialize (DataMsg* aMsg);

   static void show (TestMsg* aMsg);
   static void show (FirstMessageMsg* aMsg);
   static void show (StatusRequestMsg* aMsg);
   static void show (StatusResponseMsg* aMsg);
   static void show (DataRecord* aMsg);
   static void show (DataMsg* aMsg);

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
