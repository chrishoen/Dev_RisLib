#ifndef _EXAMPLEMSGHELPER_H_
#define _EXAMPLEMSGHELPER_H_

#include "exampleMsg.h"

namespace ExampleMsg
{

    //***************************************************************************
    //***************************************************************************
    //***************************************************************************

    class MsgHelper
    {
    public:

       static void initialize (TestMsg*   aMsg);
       static void initialize (StatusMsg* aMsg);
       static void initialize (Data1Msg*  aMsg);
       static void initialize (Data2Msg*  aMsg);
       static void initialize (Data3Msg*  aMsg);
       static void initialize (Data4Msg*  aMsg);

       static void show (TestMsg*   aMsg);
       static void show (StatusMsg* aMsg);
       static void show (Data1Msg*  aMsg);
       static void show (Data2Msg*  aMsg);
       static void show (Data3Msg*  aMsg);
       static void show (Data4Msg*  aMsg);

    };


}
#endif
