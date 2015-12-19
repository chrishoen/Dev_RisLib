#include <stdio.h>
#include <string.h>
#include "greenRecord.h"

namespace GreenRecord
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
      case MsgIdT::cTestRecord :
         tRecord = new TestRecord;
         break;
      case MsgIdT::cStatusRecord :
         tRecord = new StatusRecord;
         break;
      case MsgIdT::cData1Record :
         tRecord = new Data1Record;
         break;
      case MsgIdT::cData2Record :
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
      case MsgIdT::cTestRecord :
         copyToFrom(aBuffer,(TestRecord*)aRecord);
         break;
      case MsgIdT::cStatusRecord :
         copyToFrom(aBuffer,(StatusRecord*)aRecord);
         break;
      case MsgIdT::cData1Record :
         copyToFrom(aBuffer,(Data1Record*)aRecord);
         break;
      case MsgIdT::cData2Record :
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
      mRecordType     = MsgIdT::cTestRecord;

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

   void TestRecord::initialize()
   {
      mCode1 = 901;
      mCode2 = 902;
      mCode3 = 903;
      mCode4 = 904;
   }

   void TestRecord::show()
   {
      printf("%d\n",  mCode1   );
      printf("%d\n",  mCode1   );
      printf("%d\n",  mCode2   );
      printf("%d\n",  mCode3   );
      printf("\n");
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // StatusRecord

   StatusRecord::StatusRecord()
   {
      mRecordType     = MsgIdT::cStatusRecord;

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
      mRecordType     = MsgIdT::cData1Record;

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

   void Data1Record::initialize()
   {
      mCode1 = 101;
      mCode2 = 102;
      mCode3 = 103;
      mCode4 = 104;
   }

   void Data1Record::show()
   {
      printf("%d\n",  mCode1   );
      printf("%d\n",  mCode1   );
      printf("%d\n",  mCode2   );
      printf("%d\n",  mCode3   );
      printf("\n");
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data2Record

   Data2Record::Data2Record()
   {
      mRecordType     = MsgIdT::cData2Record;

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
   }

   void Data2Record::initialize()
   {
      mCode1 = 201;
      mCode2 = 202;
      mCode3 = 203;
      mCode4 = 204;
   }

   void Data2Record::show()
   {
      printf("%d\n",  mCode1   );
      printf("%d\n",  mCode1   );
      printf("%d\n",  mCode2   );
      printf("%d\n",  mCode3   );
      printf("\n");
   }
}
