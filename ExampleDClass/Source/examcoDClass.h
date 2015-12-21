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
        static const int cTest   = 1;
        static const int cStatus = 2;
        static const int cData1  = 3;
        static const int cData2  = 4;
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Test
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

       Test();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Status
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

       Status();
    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data1
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

       Data1();

    };


    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Data2
    {
    public:

       //***********************************************************************
       // Members

       int   mCode1;
       int   mCode2;
       int   mCode3;
       int   mCode4;
       Data1 mData1;

       //***********************************************************************
       // Constructor

       Data2();

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class DClassCopier : public Ris::BaseDClassCopier
    {
    public:

       //***********************************************************************
       // This creates a new record, based on a record type

       Ris::DHandle createDClass(int aType);

       //***********************************************************************
       // This copies byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, Ris::DHandle aHandle);

       //***********************************************************************
       // These copy byte buffers to/from records

       void copyToFrom (Ris::ByteBuffer* aBuffer, Test*   aClass);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Status* aClass);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data1*  aClass);
       void copyToFrom (Ris::ByteBuffer* aBuffer, Data2*  aClass);

    };

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class DHMake
    {
    public:

       static Ris::DHandle handle (Test*   aClass) {return Ris::DHandle(TypeIdT::cTest,   aClass);}
       static Ris::DHandle handle (Status* aClass) {return Ris::DHandle(TypeIdT::cStatus, aClass);}
       static Ris::DHandle handle (Data1*  aClass) {return Ris::DHandle(TypeIdT::cData1,  aClass);}
       static Ris::DHandle handle (Data2*  aClass) {return Ris::DHandle(TypeIdT::cData2,  aClass);}

       static Test*   ptr_Test   (Ris::DHandle aHandle) {return (Test*)aHandle.mPtr;}
       static Status* ptr_Status (Ris::DHandle aHandle) {return (Status*)aHandle.mPtr;}
       static Data1*  ptr_Data1  (Ris::DHandle aHandle) {return (Data1*)aHandle.mPtr;}
       static Data2*  ptr_Data2  (Ris::DHandle aHandle) {return (Data2*)aHandle.mPtr;}
    };
}
#endif
