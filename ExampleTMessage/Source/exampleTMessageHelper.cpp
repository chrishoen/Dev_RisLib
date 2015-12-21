#include <stdio.h>
#include <string.h>
#include "prnPrint.h"
#include "exampleTMessageHelper.h"

namespace Example
{
   //****************************************************************************

   void TMessageHelper::initialize(TestMsg* aMsg)
   {
      aMsg->mCode1 = 901;
      aMsg->mCode2 = 902;
      aMsg->mCode3 = 903;
      aMsg->mCode4 = 904;
   }

   void TMessageHelper::show(TestMsg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void TMessageHelper::initialize(StatusMsg* aMsg)
   {
      aMsg->mCode1 = 901;
      aMsg->mCode2 = 902;
      aMsg->mCode3 = 903;
      aMsg->mCode4 = 904;
   }

   void TMessageHelper::show(StatusMsg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void TMessageHelper::initialize(Data1Msg* aMsg)
   {
      aMsg->mCode1 = 101;
      aMsg->mCode2 = 102;
      aMsg->mCode3 = 103;
      aMsg->mCode4 = 104;
   }

   void TMessageHelper::show(Data1Msg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "");
   }


   //****************************************************************************

   void TMessageHelper::initialize(Data2Msg* aMsg)
   {
      aMsg->mCode1 = 201;
      aMsg->mCode2 = 202;
      aMsg->mCode3 = 203;
      aMsg->mCode4 = 204;
      TMessageHelper::initialize(&aMsg->mTMessage1);
   }

   void TMessageHelper::show(Data2Msg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      TMessageHelper::show(&aMsg->mTMessage1);
      Prn::print(Prn::ThreadRun3, "");
   }

   //****************************************************************************

   void TMessageHelper::initialize(Data3Msg* aMsg)
   {
      aMsg->mCode1 = 201;
      aMsg->mCode2 = 202;
      aMsg->mCode3 = 203;
      aMsg->mCode4 = 204;
      strcpy(aMsg->mString1,"ABCDEFGH");
      aMsg->mCode5Loop = 4;
      for (int i = 0; i < 4; i++)
      {
      aMsg->mCode5[i]=1001+i;
      }
   }

   void TMessageHelper::show(Data3Msg* aMsg)
   {
      Prn::print(Prn::ThreadRun3, "Code1      %d", aMsg->mCode1);
      Prn::print(Prn::ThreadRun3, "Code2      %d", aMsg->mCode2);
      Prn::print(Prn::ThreadRun3, "Code3      %d", aMsg->mCode3);
      Prn::print(Prn::ThreadRun3, "Code4      %d", aMsg->mCode4);
      Prn::print(Prn::ThreadRun3, "String1    %s", aMsg->mString1);
      aMsg->mCode5Loop = 4;
      for (int i = 0; i < 4; i++)
      {
      Prn::print(Prn::ThreadRun3, "Code5      %d", aMsg->mCode5[i]);
      }
      Prn::print(Prn::ThreadRun3, "");
   }
}//namespace
