#ifndef _EXAMCOMHELPER_H_
#define _EXAMCOHELPER_H_

#include "examcoRecord.h"

namespace ExampleComm
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Helper
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
