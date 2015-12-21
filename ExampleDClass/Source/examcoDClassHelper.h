#ifndef _EXAMCODCLASSHELPER_H_
#define _EXAMCODCLASSHELPER_H_

#include "examcoDClass.h"

namespace ExampleComm
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class Helper
    {
    public:

       static void initialize (Test*   aClass);
       static void initialize (Status* aClass);
       static void initialize (Data1*  aClass);
       static void initialize (Data2*  aClass);

       static void show (Test*   aClass);
       static void show (Status* aClass);
       static void show (Data1*  aClass);
       static void show (Data2*  aClass);

    };


}
#endif
