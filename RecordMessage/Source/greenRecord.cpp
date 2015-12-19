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

}
