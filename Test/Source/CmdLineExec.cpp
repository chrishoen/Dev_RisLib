#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_functions.h"
#include "risPortableCalls.h"
#include "prnPrint.h"

#include "CmdLineExec.h"

//******************************************************************************
CmdLineExec::CmdLineExec()
{
}
//******************************************************************************
void CmdLineExec::reset()
{
}
//******************************************************************************
void CmdLineExec::execute(Ris::CmdLineCmd* aCmd)
{
   if(aCmd->isCmd("RESET"  ))  reset();
   if(aCmd->isCmd("GO1"    ))  executeGo1(aCmd);
   if(aCmd->isCmd("GO2"    ))  executeGo2(aCmd);
   if(aCmd->isCmd("GO3"    ))  executeGo3(aCmd);
   if(aCmd->isCmd("GO4"    ))  executeGo4(aCmd);
   if(aCmd->isCmd("GO5"    ))  executeGo5(aCmd);
}

void my_function1(int aNumArgs, ...)
{
   va_list valist;
   va_start(valist,aNumArgs);
   for (int i=0;i<aNumArgs;i++)
   {
      int tIndex = va_arg(valist,int);
      Prn::print(0,"index %d",tIndex);
   }
   va_end(valist);

}

//******************************************************************************
void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   my_function1(2,11,12);
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,1000);

   int tTimeout = aCmd->argInt(1);
   int tTimeout10 = tTimeout/100;

   Prn::print(0,"%5d %5d",tTimeout,tTimeout10);
}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,"abcdefg");

   Prn::print(0,"%s %s",
      my_string_from_bool(aCmd->isArgString(1, "abcdefg")),
      aCmd->argString(1));
}

//******************************************************************************

void CmdLineExec::executeGo4(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(1,0);
   aCmd->setArgDefault(2,1);

   int tIndex = aCmd->argInt(1);
   int tDelta = aCmd->argInt(2);
   int tSize  = 5;

   Prn::print(0,"");
   Prn::print(0,"index %d",tIndex);
   Prn::print(0,"delta %d",tDelta);
   Prn::print(0,"");

   tIndex = my_index_add(tIndex,tDelta,tSize);

   Prn::print(0,"index %d",tIndex);
   Prn::print(0,"");
}

//******************************************************************************

class MyClass1
{
public:

   MyClass1()
   {
      mX1[0] = 101;
      mX2[0] = 102;
   }

   enum {MaxSize = 100};
   int  mX1[MaxSize];

   static const int cMaxSize = 100;
   int  mX2[cMaxSize];

   void myCall(int aX)
   {
      const int* tPtr1 = &cMaxSize;
      int* tPtr2 = (int*)&cMaxSize;
   }

};

void CmdLineExec::executeGo5(Ris::CmdLineCmd* aCmd)
{
}

