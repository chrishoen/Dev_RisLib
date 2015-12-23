#ifndef _EXAMPLETMESSAGE_H_
#define _EXAMPLETMESSAGE_H_

#include "risByteBuffer.h"
#include "risByteTMessage.h"
#include "risByteTMessageCopier.h"

namespace Example
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************
    // TMessage Types

    class TypeIdT
    {
    public:
        static const int cUnspecified  = 0;
        static const int cTestMsg   = 1;
        static const int cStatusMsg = 2;
        static const int cData1Msg  = 3;
        static const int cData2Msg  = 4;
        static const int cData3Msg  = 5;
        static const int cData4Msg  = 6;
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class TestMsg : public Ris::ByteTMessage
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;

       //***********************************************************************
       // Constructor

       TestMsg();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class StatusMsg : public Ris::ByteTMessage
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;

       //***********************************************************************
       // Constructor

       StatusMsg();
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data1Msg : public Ris::ByteTMessage
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;

       //***********************************************************************
       // Constructor

       Data1Msg();

    };


    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data2Msg : public Ris::ByteTMessage
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;
       Data1Msg mData1;

       //***********************************************************************
       // Constructor

       Data2Msg();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data3Msg : public Ris::ByteTMessage
    {
    public:

       //***********************************************************************
       // Members

       int       mCode1;
       int       mCode2;
       int       mCode3;
       int       mCode4;
       char      mString1[40];

       static const int cMaxLoop=4;
       int       mCode5Loop;
       int       mCode5[cMaxLoop];

       //***********************************************************************
       // Constructor

       Data3Msg();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data4Msg : public Ris::ByteTMessage
    {
    public:

       //***********************************************************************
       // Members

       int       mCode1;
       int       mCode2;
       int       mCode3;
       int       mCode4;

       static const int cMaxLoop=4;
       int       mData1Loop;
       Data1Msg  mData1[cMaxLoop];

       //***********************************************************************
       // Constructor

       Data4Msg();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class TMessageCopier : public Ris::BaseTMessageCopier
    {
    public:

       //***********************************************************************
       // This creates a new record, based on a record type

       Ris::ByteTMessage* createMessage(int aMsgType);

       //***********************************************************************
       // This copies byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteTMessage* aMsg);

       //***********************************************************************
       // These copy byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, TestMsg*   aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, StatusMsg* aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data1Msg*  aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data2Msg*  aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data3Msg*  aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data4Msg*  aMsg);

    };
}
#endif