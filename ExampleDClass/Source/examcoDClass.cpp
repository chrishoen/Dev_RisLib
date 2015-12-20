#include <stdio.h>
#include <string.h>
#include "examcoDClass.h"

namespace ExampleComm
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This creates a new record, based on a record type

   Ris::ByteDClass* DClassCopier::createDClass(int aDClassType)
   {
      Ris::ByteDClass* tDClass = 0;

      switch (aDClassType)
      {
      case TypeIdT::cTestDClass :
         tDClass = new TestDClass;
         break;
      case TypeIdT::cStatusDClass :
         tDClass = new StatusDClass;
         break;
      case TypeIdT::cData1DClass :
         tDClass = new Data1DClass;
         break;
      case TypeIdT::cData2DClass :
         tDClass = new Data2DClass;
         break;
      default :
         return 0;
         break;
      }

      return tDClass;
   }
   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // DClassCopier

   void DClassCopier::copyToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteDClass* aDClass)
   {
      switch (aDClass->mDClassType)
      {
      case TypeIdT::cTestDClass :
         copyToFrom(aBuffer,(TestDClass*)aDClass);
         break;
      case TypeIdT::cStatusDClass :
         copyToFrom(aBuffer,(StatusDClass*)aDClass);
         break;
      case TypeIdT::cData1DClass :
         copyToFrom(aBuffer,(Data1DClass*)aDClass);
         break;
      case TypeIdT::cData2DClass :
         copyToFrom(aBuffer,(Data2DClass*)aDClass);
         break;
      default :
         break;
      }
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // TestDClass

   TestDClass::TestDClass()
   {
      mDClassType     = TypeIdT::cTestDClass;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, TestDClass* aDClass)
   {
      aBuffer->copy( &aDClass->mCode1 );
      aBuffer->copy( &aDClass->mCode2 );
      aBuffer->copy( &aDClass->mCode3 );
      aBuffer->copy( &aDClass->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // StatusDClass

   StatusDClass::StatusDClass()
   {
      mDClassType     = TypeIdT::cStatusDClass;

      mCode1           = 1002;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, StatusDClass* aDClass)
   {
      aBuffer->copy( &aDClass->mCode1 );
      aBuffer->copy( &aDClass->mCode2 );
      aBuffer->copy( &aDClass->mCode3 );
      aBuffer->copy( &aDClass->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data1DClass

   Data1DClass::Data1DClass()
   {
      mDClassType     = TypeIdT::cData1DClass;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data1DClass* aDClass)
   {
      aBuffer->copy( &aDClass->mCode1 );
      aBuffer->copy( &aDClass->mCode2 );
      aBuffer->copy( &aDClass->mCode3 );
      aBuffer->copy( &aDClass->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data2DClass

   Data2DClass::Data2DClass()
   {
      mDClassType     = TypeIdT::cData2DClass;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void DClassCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data2DClass* aDClass)
   {
      aBuffer->copy( &aDClass->mCode1 );
      aBuffer->copy( &aDClass->mCode2 );
      aBuffer->copy( &aDClass->mCode3 );
      aBuffer->copy( &aDClass->mCode4 );
      DClassCopier::copyToFrom   (aBuffer, &aDClass->mDClass1 );
   }

}
