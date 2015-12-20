#ifndef _EXAMCOMSG_DCLASS_H_
#define _EXAMCOMSG_DCLASS_H_

#include "risByteBuffer.h"
#include "risByteDClass.h"
#include "risByteDClassCopier.h"

namespace ExampleComm
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************
    // DClass Types

    class TypeIdT
    {
    public:
        static const int cUnspecified  = 0;
        static const int cTestDClass   = 1;
        static const int cStatusDClass = 2;
        static const int cData1DClass  = 3;
        static const int cData2DClass  = 4;
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class TestDClass : public Ris::ByteDClass
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

       TestDClass();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class StatusDClass : public Ris::ByteDClass
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

       StatusDClass();
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data1DClass : public Ris::ByteDClass
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

       Data1DClass();

    };


    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data2DClass : public Ris::ByteDClass
    {
    public:

       //***********************************************************************
       // Members

       int mCode1;
       int mCode2;
       int mCode3;
       int mCode4;
       Data1DClass mDClass1;

       //***********************************************************************
       // Constructor

       Data2DClass();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class DClassCopier : public Ris::BaseDClassCopier
    {
    public:

       //***********************************************************************
       // This creates a new record, based on a record type

       Ris::ByteDClass* createDClass(int aDClassType);

       //***********************************************************************
       // This copies byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, Ris::ByteDClass* aDClass);

       //***********************************************************************
       // These copy byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, TestDClass*   aDClass);
       void copyToFrom (Ris::ByteBuffer* aBuffer, StatusDClass* aDClass);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data1DClass*  aDClass);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data2DClass*  aDClass);

    };


}
#endif
