#include <stdio.h>
#include <string.h>
#include "exampleTMessage.h"

namespace Example
{
   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // This creates a new record, based on a record type

   Ris::ByteTMessage* TMessageCopier::createMessage(int aMsgType)
   {
      Ris::ByteTMessage* tTMessage = 0;

      switch (aMsgType)
      {
      case TypeIdT::cTestMsg :
         tTMessage = new TestMsg;
         break;
      case TypeIdT::cStatusMsg :
         tTMessage = new StatusMsg;
         break;
      case TypeIdT::cData1Msg :
         tTMessage = new Data1Msg;
         break;
      case TypeIdT::cData2Msg :
         tTMessage = new Data2Msg;
         break;
      case TypeIdT::cData3Msg :
         tTMessage = new Data3Msg;
         break;
      case TypeIdT::cData4Msg :
         tTMessage = new Data4Msg;
         break;
      default :
         return 0;
         break;
      }

      return tTMessage;
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // TMessageCopier

   void TMessageCopier::copyToFrom(Ris::ByteBuffer* aBuffer, Ris::ByteTMessage* aMsg)
   {
      switch (aMsg->mTMessageType)
      {
      case TypeIdT::cTestMsg :
         copyToFrom(aBuffer,(TestMsg*)aMsg);
         break;
      case TypeIdT::cStatusMsg :
         copyToFrom(aBuffer,(StatusMsg*)aMsg);
         break;
      case TypeIdT::cData1Msg :
         copyToFrom(aBuffer,(Data1Msg*)aMsg);
         break;
      case TypeIdT::cData2Msg :
         copyToFrom(aBuffer,(Data2Msg*)aMsg);
         break;
      case TypeIdT::cData4Msg :
         copyToFrom(aBuffer,(Data4Msg*)aMsg);
         break;
      default :
         break;
      }
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // TestMsg

   TestMsg::TestMsg()
   {
      mTMessageType     = TypeIdT::cTestMsg;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void TMessageCopier::copyToFrom (Ris::ByteBuffer* aBuffer, TestMsg* aMsg)
   {
      aBuffer->copy( &aMsg->mCode1 );
      aBuffer->copy( &aMsg->mCode2 );
      aBuffer->copy( &aMsg->mCode3 );
      aBuffer->copy( &aMsg->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // StatusMsg

   StatusMsg::StatusMsg()
   {
      mTMessageType     = TypeIdT::cStatusMsg;

      mCode1           = 1002;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void TMessageCopier::copyToFrom (Ris::ByteBuffer* aBuffer, StatusMsg* aMsg)
   {
      aBuffer->copy( &aMsg->mCode1 );
      aBuffer->copy( &aMsg->mCode2 );
      aBuffer->copy( &aMsg->mCode3 );
      aBuffer->copy( &aMsg->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data1Msg

   Data1Msg::Data1Msg()
   {
      mTMessageType     = TypeIdT::cData1Msg;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void TMessageCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data1Msg* aMsg)
   {
      aBuffer->copy( &aMsg->mCode1 );
      aBuffer->copy( &aMsg->mCode2 );
      aBuffer->copy( &aMsg->mCode3 );
      aBuffer->copy( &aMsg->mCode4 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data2Msg

   Data2Msg::Data2Msg()
   {
      mTMessageType     = TypeIdT::cData2Msg;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
   }

   void TMessageCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data2Msg* aMsg)
   {
      aBuffer->copy( &aMsg->mCode1 );
      aBuffer->copy( &aMsg->mCode2 );
      aBuffer->copy( &aMsg->mCode3 );
      aBuffer->copy( &aMsg->mCode4 );
      TMessageCopier::copyToFrom   (aBuffer, &aMsg->mData1 );
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data3Msg

   Data3Msg::Data3Msg()
   {
      mTMessageType     = TypeIdT::cData3Msg;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;

      mString1[0]=0;

      mCode5Loop=cMaxLoop;
      for (int i = 0; i < cMaxLoop; i++)
      {
         mCode5[i]=0;
      }
   }

   void TMessageCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data3Msg* aMsg)
   {
      aBuffer->copy( &aMsg->mCode1 );
      aBuffer->copy( &aMsg->mCode2 );
      aBuffer->copy( &aMsg->mCode3 );
      aBuffer->copy( &aMsg->mCode4 );

      aBuffer->copyS( aMsg->mString1 );

      aBuffer->copy( &aMsg->mCode5Loop );
      for (int i=0; i<aMsg->mCode5Loop; i++)
      {
      aBuffer->copy( &aMsg->mCode5[i]);
      }
   }

   //****************************************************************************
   //****************************************************************************
   //****************************************************************************
   // Data4Msg

   Data4Msg::Data4Msg()
   {
      mTMessageType     = TypeIdT::cData4Msg;

      mCode1           = 1001;
      mCode2           = 0;
      mCode3           = 0;
      mCode4           = 0;
 
      mData1Loop=cMaxLoop;
   }

   void TMessageCopier::copyToFrom (Ris::ByteBuffer* aBuffer, Data4Msg* aMsg)
   {
      aBuffer->copy( &aMsg->mCode1 );
      aBuffer->copy( &aMsg->mCode2 );
      aBuffer->copy( &aMsg->mCode3 );
      aBuffer->copy( &aMsg->mCode4 );

      aBuffer->copy( &aMsg->mData1Loop );
      for (int i=0; i<aMsg->mData1Loop; i++)
      {
      TMessageCopier::copyToFrom   (aBuffer, &aMsg->mData1[i] );
      }
   }

}
