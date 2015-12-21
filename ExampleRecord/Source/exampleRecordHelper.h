#ifndef _EXAMPLERECORDHELPER_H_
#define _EXAMPLERECORDHELPER_H__

#include "exampleRecord.h"

namespace Example
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class RecordHelper
    {
    public:

       static void initialize (TestRecord*   aRecord);
       static void initialize (StatusRecord* aRecord);
       static void initialize (Data1Record*  aRecord);
       static void initialize (Data2Record*  aRecord);

       static void show (TestRecord*   aRecord);
       static void show (StatusRecord* aRecord);
       static void show (Data1Record*  aRecord);
       static void show (Data2Record*  aRecord);

    };


}
#endif
