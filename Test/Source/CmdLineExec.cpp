#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <atomic>
#include <bitset>

#include "my_functions.h"
#include "risPortableCalls.h"
#include "risLogic.h"
#include "prnPrint.h"

#include "CmdLineExec.h"

using namespace std;
//******************************************************************************
// change101
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


//******************************************************************************

void setBit(atomic<unsigned>& aX,int aBit)
{
   aX |= (1 << aBit);
}

void clearBit(atomic<unsigned>& aX,int aBit)
{
   aX &= ~(1 << aBit);
}

void CmdLineExec::executeGo1(Ris::CmdLineCmd* aCmd)
{
   atomic<unsigned> tX;
   Prn::print(0,"%d",sizeof(tX));

   tX = 0;

   setBit(tX,0);
   setBit(tX,2);
   Prn::print(0,"%08X",tX);

   clearBit(tX,0);
   Prn::print(0,"%08X",tX);
}

//******************************************************************************

void CmdLineExec::executeGo2(Ris::CmdLineCmd* aCmd)
{
   atomic<unsigned> tMask=0;
   atomic<unsigned> tLatch=0;
   unsigned         tCompare=0;

   setBit(tMask,0);
   setBit(tMask,1);
   setBit(tMask,2);
   Prn::print(0,"Mask    %08X",tMask);

   setBit(tLatch,1);
   setBit(tLatch,2);
   setBit(tLatch,3);
   Prn::print(0,"Latch   %08X",tLatch);

   tCompare = tMask & tLatch;
   Prn::print(0,"Compare %08X",tCompare);

   bool tAny = tCompare != 0;
   bool tAll = tCompare == tMask;
   Prn::print(0,"Any %d",tAny);
   Prn::print(0,"All %d",tAll);

}

//******************************************************************************

void CmdLineExec::executeGo3(Ris::CmdLineCmd* aCmd)
{
   Ris::Logic::AndOrLatch tLatch;

   tLatch.setMaskBit  (0);
   tLatch.setMaskBit  (1);

   tLatch.setLatchBit (1);
   tLatch.setLatchBit (2);

   Prn::print(0,"Any %d",tLatch.isAny());
   Prn::print(0,"All %d",tLatch.isAll());
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

