#ifndef _EXAMPLEMSG_H_
#define _EXAMPLEMSG_H_

#include "risByteBuffer.h"
#include "risByteMsgB.h"

namespace ExampleMsg
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************
    // MsgB Types

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

    class TestMsg : public Ris::ByteMsgB
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

    class StatusMsg : public Ris::ByteMsgB
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

    class Data1Msg : public Ris::ByteMsgB
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

    class Data2Msg : public Ris::ByteMsgB
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

    class Data3Msg : public Ris::ByteMsgB
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

    class Data4Msg : public Ris::ByteMsgB
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

    class MsgCreator : public Ris::BaseMsgBCreator
    {
    public:

       //***********************************************************************
       // Create a new record, based on a record type

       Ris::ByteMsgB* createMessage(int aMsgType) override;

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class MsgCopier : public Ris::BaseMsgBCopier
    {
    public:

       //***********************************************************************
       // Copy a record to/from a byte buffer. 

       void copyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteMsgB* aMsg) override;

       //***********************************************************************
       // Copy a record to/from a byte buffer. 

       void copyToFrom (Ris::ByteBuffer* aBuffer, TestMsg*   aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, StatusMsg* aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data1Msg*  aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data2Msg*  aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data3Msg*  aMsg);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data4Msg*  aMsg);

    };
}
#endif
