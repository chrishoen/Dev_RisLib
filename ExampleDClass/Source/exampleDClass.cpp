#include <stdio.h>
#include <string.h>
#include "exampleDClass.h"

namespace Example
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This creates a new record, based on a record type

   DHandle DClassCopier::createDClass(int aType)
   {
      DHandle tHandle;

      switch (aType)
      {
      case TypeIdT::cTest :
         return DHandle(TypeIdT::cTest,   new Test);
         break;
      case TypeIdT::cStatus :
         return DHandle(TypeIdT::cStatus, new Status);
         break;
      case TypeIdT::cData1 :
         return DHandle(TypeIdT::cData1,  new Data1);
         break;
      case TypeIdT::cData2 :
         return DHandle(TypeIdT::cData2,  new Data2);
         break;
      }

      return tHandle;
   }
   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Copier

   void DClassCopier::copyToFrom(Ris::ByteBuffer* aBuffer, DHandle aHandle)
   {
      switch (aHandle.mType)
      {
      case TypeIdT::cTest :
         copyToFrom(aBuffer,(Test*)aHandle.mPtr);
         break;
      case TypeIdT::cStatus :
         copyToFrom(aBuffer,(Status*)aHandle.mPtr);
         break;
      case TypeIdT::cData1 :
         copyToFrom(aBuffer,(Data1*)aHandle.mPtr);
         break;
      case TypeIdT::cData2 :
         copyToFrom(aBuffer,(Data2*)aHandle.mPtr);
         break;
      default :
         break;
      }
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Test

   Test::Test()
   {
      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Test* aClass)
   {
      aBuffer->copy( &aClass->mCode1 );
      aBuffer->copy( &aClass->mCode2 );
      aBuffer->copy( &aClass->mCode3 );
      aBuffer->copy( &aClass->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Status

   Status::Status()
   {
      mCode1           = 1002;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Status* aClass)
   {
      aBuffer->copy( &aClass->mCode1 );
      aBuffer->copy( &aClass->mCode2 );
      aBuffer->copy( &aClass->mCode3 );
      aBuffer->copy( &aClass->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data1

   Data1::Data1()
   {
      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data1* aClass)
   {
      aBuffer->copy( &aClass->mCode1 );
      aBuffer->copy( &aClass->mCode2 );
      aBuffer->copy( &aClass->mCode3 );
      aBuffer->copy( &aClass->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data2

   Data2::Data2()
   {
      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data2* aClass)
   {
      aBuffer->copy( &aClass->mCode1 );
      aBuffer->copy( &aClass->mCode2 );
      aBuffer->copy( &aClass->mCode3 );
      aBuffer->copy( &aClass->mCode4 );
      DClassCopier::copyToFrom   (aBuffer, &aClass->mData1 );
   }

}
