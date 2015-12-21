#include <stdio.h>
#include <string.h>
#include "prnPrint.h"
#include "exampleRecordHelper.h"

namespace Example
{
   //****************************************************************************

   void RecordHelper::initialize(TestRecord* aRecord)
   {
      aRecord->mCode1 = 901;
      aRecord->mCode2 = 902;
      aRecord->mCode3 = 903;
      aRecord->mCode4 = 904;
   }

   void RecordHelper::show(TestRecord* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void RecordHelper::initialize(StatusRecord* aRecord)
   {
      aRecord->mCode1 = 901;
      aRecord->mCode2 = 902;
      aRecord->mCode3 = 903;
      aRecord->mCode4 = 904;
   }

   void RecordHelper::show(StatusRecord* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void RecordHelper::initialize(Data1Record* aRecord)
   {
      aRecord->mCode1 = 101;
      aRecord->mCode2 = 102;
      aRecord->mCode3 = 103;
      aRecord->mCode4 = 104;
   }

   void RecordHelper::show(Data1Record* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }


   //****************************************************************************

   void RecordHelper::initialize(Data2Record* aRecord)
   {
      aRecord->mCode1 = 201;
      aRecord->mCode2 = 202;
      aRecord->mCode3 = 203;
      aRecord->mCode4 = 204;
      RecordHelper::initialize(&aRecord->mRecord1);
   }

   void RecordHelper::show(Data2Record* aRecord)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aRecord->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aRecord->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aRecord->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aRecord->mCode4);
      RecordHelper::show(&aRecord->mRecord1);
      Prn::print(Prn::ThreadRun3, "");
   }
}//namespace
