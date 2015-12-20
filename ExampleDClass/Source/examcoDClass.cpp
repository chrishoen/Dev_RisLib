#include <stdio.h>
#include <string.h>
#include "examcoDClass.h"

namespace ExampleComm
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This creates a new record, based on a record type

   Ris::DHandle DClassCopier::createDClass(int aType)
   {
      Ris::DHandle tHandle;

      switch (aType)
      {
      case TypeIdT::cTest :
         return Ris::DHandle(TypeIdT::cTest,   new Test);
         break;
      case TypeIdT::cStatus :
         return Ris::DHandle(TypeIdT::cStatus, new Status);
         break;
      case TypeIdT::cData1 :
         return Ris::DHandle(TypeIdT::cData1,  new Data1);
         break;
      case TypeIdT::cData2 :
         return Ris::DHandle(TypeIdT::cData2,  new Data2);
         break;
      }

      return tHandle;
   }
   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Copier

   void DClassCopier::copyToFrom(Ris::ByteBuffer* aBuffer, Ris::DHandle aHandle)
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
