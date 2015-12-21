#include <stdio.h>
#include <string.h>
#include "prnPrint.h"
#include "exampleDClassHelper.h"

namespace Example
{
   //****************************************************************************

   void Helper::initialize(Test* aClass)
   {
      aClass->mCode1 = 901;
      aClass->mCode2 = 902;
      aClass->mCode3 = 903;
      aClass->mCode4 = 904;
   }

   void Helper::show(Test* aClass)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aClass->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aClass->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aClass->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aClass->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void Helper::initialize(Status* aClass)
   {
      aClass->mCode1 = 901;
      aClass->mCode2 = 902;
      aClass->mCode3 = 903;
      aClass->mCode4 = 904;
   }

   void Helper::show(Status* aClass)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aClass->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aClass->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aClass->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aClass->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void Helper::initialize(Data1* aClass)
   {
      aClass->mCode1 = 101;
      aClass->mCode2 = 102;
      aClass->mCode3 = 103;
      aClass->mCode4 = 104;
   }

   void Helper::show(Data1* aClass)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aClass->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aClass->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aClass->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aClass->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }


   //****************************************************************************

   void Helper::initialize(Data2* aClass)
   {
      aClass->mCode1 = 201;
      aClass->mCode2 = 202;
      aClass->mCode3 = 203;
      aClass->mCode4 = 204;
      Helper::initialize(&aClass->mData1);
   }

   void Helper::show(Data2* aClass)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aClass->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aClass->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aClass->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aClass->mCode4);
      Helper::show(&aClass->mData1);
      Prn::print(Prn::ThreadRun3, "");
   }
}//namespace
