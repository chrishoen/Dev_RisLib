#include <stdio.h>
#include <string.h>
#include "examcoRecord.h"

namespace ExampleComm
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This creates a new record, based on a record type

   Ris::ByteRecord* RecordCopier::createRecord(int aRecordType)
   {
      Ris::ByteRecord* tRecord = 0;

      switch (aRecordType)
      {
      case TypeIdT::cTestRecord :
         tRecord = new TestRecord;
         break;
      case TypeIdT::cStatusRecord :
         tRecord = new StatusRecord;
         break;
      case TypeIdT::cData1Record :
         tRecord = new Data1Record;
         break;
      case TypeIdT::cData2Record :
         tRecord = new Data2Record;
         break;
      default :
         return 0;
         break;
      }

      return tRecord;
   }
   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // RecordCopier

   void RecordCopier::copyToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteRecord* aRecord)
   {
      switch (aRecord->mRecordType)
      {
      case TypeIdT::cTestRecord :
         copyToFrom(aBuffer,(TestRecord*)aRecord);
         break;
      case TypeIdT::cStatusRecord :
         copyToFrom(aBuffer,(StatusRecord*)aRecord);
         break;
      case TypeIdT::cData1Record :
         copyToFrom(aBuffer,(Data1Record*)aRecord);
         break;
      case TypeIdT::cData2Record :
         copyToFrom(aBuffer,(Data2Record*)aRecord);
         break;
      default :
         break;
      }
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // TestRecord

   TestRecord::TestRecord()
   {
      mRecordType     = TypeIdT::cTestRecord;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void RecordCopier::copyToFrom (Ris::ByteBuffer* aBuffer, TestRecord* aRecord)
   {
      aBuffer->copy( &aRecord->mCode1 );
      aBuffer->copy( &aRecord->mCode2 );
      aBuffer->copy( &aRecord->mCode3 );
      aBuffer->copy( &aRecord->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // StatusRecord

   StatusRecord::StatusRecord()
   {
      mRecordType     = TypeIdT::cStatusRecord;

      mCode1           = 1002;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void RecordCopier::copyToFrom (Ris::ByteBuffer* aBuffer, StatusRecord* aRecord)
   {
      aBuffer->copy( &aRecord->mCode1 );
      aBuffer->copy( &aRecord->mCode2 );
      aBuffer->copy( &aRecord->mCode3 );
      aBuffer->copy( &aRecord->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data1Record

   Data1Record::Data1Record()
   {
      mRecordType     = TypeIdT::cData1Record;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void RecordCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data1Record* aRecord)
   {
      aBuffer->copy( &aRecord->mCode1 );
      aBuffer->copy( &aRecord->mCode2 );
      aBuffer->copy( &aRecord->mCode3 );
      aBuffer->copy( &aRecord->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data2Record

   Data2Record::Data2Record()
   {
      mRecordType     = TypeIdT::cData2Record;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void RecordCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data2Record* aRecord)
   {
      aBuffer->copy( &aRecord->mCode1 );
      aBuffer->copy( &aRecord->mCode2 );
      aBuffer->copy( &aRecord->mCode3 );
      aBuffer->copy( &aRecord->mCode4 );
      RecordCopier::copyToFrom   (aBuffer, &aRecord->mRecord1 );
   }

}
