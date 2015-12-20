#include <stdio.h>
#include <string.h>
#include "prnPrint.h"
#include "examcoHelper.h"

namespace ExampleComm
{
   //****************************************************************************

   void Helper::initialize(TestRecord* aRecord)
   {
      aRecord->mCode1 = 901;
      aRecord->mCode2 = 902;
      aRecord->mCode3 = 903;
      aRecord->mCode4 = 904;
   }

   void Helper::show(TestRecord* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void Helper::initialize(StatusRecord* aRecord)
   {
      aRecord->mCode1 = 901;
      aRecord->mCode2 = 902;
      aRecord->mCode3 = 903;
      aRecord->mCode4 = 904;
   }

   void Helper::show(StatusRecord* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void Helper::initialize(Data1Record* aRecord)
   {
      aRecord->mCode1 = 101;
      aRecord->mCode2 = 102;
      aRecord->mCode3 = 103;
      aRecord->mCode4 = 104;
   }

   void Helper::show(Data1Record* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }


   //****************************************************************************

   void Helper::initialize(Data2Record* aRecord)
   {
      aRecord->mCode1 = 201;
      aRecord->mCode2 = 202;
      aRecord->mCode3 = 203;
      aRecord->mCode4 = 204;
      Helper::initialize(&aRecord->mRecord1);
   }

   void Helper::show(Data2Record* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      Helper::show(&aRecord->mRecord1);
      Prn::print(Prn::ThreadRun3, "");
   }
}//namespace
