#include <stdio.h>
#include <string.h>
#include "greenHelper.h"

namespace GreenRecord
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
      printf("%d\n",  aRecord->mCode1   );
      printf("%d\n",  aRecord->mCode2   );
      printf("%d\n",  aRecord->mCode3   );
      printf("%d\n",  aRecord->mCode4   );
      printf("\n");
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
      printf("%d\n",  aRecord->mCode1   );
      printf("%d\n",  aRecord->mCode2   );
      printf("%d\n",  aRecord->mCode3   );
      printf("%d\n",  aRecord->mCode4   );
      printf("\n");
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
      printf("%d\n",  aRecord->mCode1   );
      printf("%d\n",  aRecord->mCode2   );
      printf("%d\n",  aRecord->mCode3   );
      printf("%d\n",  aRecord->mCode4   );
      printf("\n");
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
      printf("%d\n",  aRecord->mCode1   );
      printf("%d\n",  aRecord->mCode2   );
      printf("%d\n",  aRecord->mCode3   );
      printf("%d\n",  aRecord->mCode4   );
      Helper::show(&aRecord->mRecord1);
      printf("\n");
   }
}//namespace
