#include <stdlib.h>
#include <stdio.h>

#include "risCallPointer.h"

//******************************************************************************

//------------------------------------------------------------------------------
typedef Ris::CallPointer0<>                    CPRF0;
typedef Ris::CallPointer1<int>                 CPRF1;
typedef Ris::CallPointer2<int,int>             CPRF2;
typedef Ris::CallPointer3<int,int,int>         CPRF3;
typedef Ris::CallPointer4<int,int,int,int>     CPRF4;

typedef Ris::CallPointer0<int>                 CPRT0;
typedef Ris::CallPointer1<int,int>             CPRT1;
typedef Ris::CallPointer2<int,int,int>         CPRT2;
typedef Ris::CallPointer3<int,int,int,int>     CPRT3;
typedef Ris::CallPointer4<int,int,int,int,int> CPRT4;

class  MyClass
{
public:

   MyClass()
   {
      mCode=999;
   }

   void methodRF0()                                 { printf("methodRF0 %d $\n",             mCode); }
   void methodRF1(int aP1)                          { printf("methodRF1 %d $ %d\n",          mCode, aP1); }
   void methodRF2(int aP1,int aP2)                  { printf("methodRF2 %d $ %d %d \n",      mCode, aP1,aP2); }
   void methodRF3(int aP1,int aP2,int aP3)          { printf("methodRF3 %d $ %d %d %d\n",    mCode, aP1,aP2,aP3); }
   void methodRF4(int aP1,int aP2,int aP3,int aP4)  { printf("methodRF4 %d $ %d %d %d %d\n", mCode, aP1,aP2,aP3,aP4); }

   int  methodRT0()                                 { printf("methodRT0 %d $\n",             mCode); return 200;}
   int  methodRT1(int aP1)                          { printf("methodRT1 %d $ %d\n",          mCode, aP1); return 201;}
   int  methodRT2(int aP1,int aP2)                  { printf("methodRT2 %d $ %d %d \n",      mCode, aP1,aP2); return 202;}
   int  methodRT3(int aP1,int aP2,int aP3)          { printf("methodRT3 %d $ %d %d %d\n",    mCode, aP1,aP2,aP3); return 203;}
   int  methodRT4(int aP1,int aP2,int aP3,int aP4)  { printf("methodRT4 %d $ %d %d %d %d\n", mCode, aP1,aP2,aP3,aP4); return 204;}

   int  mCode;
};



int main(int argc,char** argv)
{
   MyClass* tMyClass = new MyClass;

   CPRF0 tCPRF0;
   CPRF1 tCPRF1;
   CPRF2 tCPRF2;
   CPRF3 tCPRF3;
   CPRF4 tCPRF4;

   tCPRF0.bind(tMyClass,&MyClass::methodRF0);
   tCPRF1.bind(tMyClass,&MyClass::methodRF1);
   tCPRF2.bind(tMyClass,&MyClass::methodRF2);
   tCPRF3.bind(tMyClass,&MyClass::methodRF3);
   tCPRF4.bind(tMyClass,&MyClass::methodRF4);

   tCPRF0();
   tCPRF1(101);
   tCPRF2(201,202);
   tCPRF3(301,302,303);
   tCPRF4(401,402,403,404);

   CPRT0 tCPRT0;
   CPRT1 tCPRT1;
   CPRT2 tCPRT2;
   CPRT3 tCPRT3;
   CPRT4 tCPRT4;

   tCPRT0.bind(tMyClass,&MyClass::methodRT0);
   tCPRT1.bind(tMyClass,&MyClass::methodRT1);
   tCPRT2.bind(tMyClass,&MyClass::methodRT2);
   tCPRT3.bind(tMyClass,&MyClass::methodRT3);
   tCPRT4.bind(tMyClass,&MyClass::methodRT4);

   int i0 = tCPRT0();
   int i1 = tCPRT1(101);
   int i2 = tCPRT2(201,202);
   int i3 = tCPRT3(301,302,303);
   int i4 = tCPRT4(401,402,403,404);

   printf("i0 %d\n",i0);
   printf("i1 %d\n",i1);
   printf("i2 %d\n",i2);
   printf("i3 %d\n",i3);
   printf("i4 %d\n",i4);

   getchar();

   return 0;
}
