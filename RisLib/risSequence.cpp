//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"


#include "risSequence.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
ThreeSequence::ThreeSequence()
{
   offi = 0;
   offj = 0;
   offk = 0;
   initialize(1, 1, 1);
}

ThreeSequence::ThreeSequence(int ami, int amj, int amk)
{
   offi = 0;
   offj = 0;
   offk = 0;
   initialize(ami, amj, amk);
}

ThreeSequence::ThreeSequence(int aoffi, int aoffj, int aoffk, int ami, int amj, int amk)
{
   offi = aoffi;
   offj = aoffj;
   offk = aoffk;
   initialize(ami, amj, amk);
}

void ThreeSequence::initialize(int aoffi, int aoffj, int aoffk, int ami, int amj, int amk)
{
   offi = aoffi;
   offj = aoffj;
   offk = aoffk;
   initialize(ami, amj, amk);
}

void ThreeSequence::restart()
{
   initialize(mi, mj, mk);
}

void ThreeSequence::initialize(int ami, int amj, int amk)
{
   mi = ami > 0 ? ami : 1;
   mj = amj > 0 ? amj : 1;
   mk = amk > 0 ? amk : 1;
   i = 0;
   j = 0;
   k = 0;
   n = 0;
   firstflag = true;
   calc_vars();
}

void ThreeSequence::advance()
{
   n++;
   i = (n / ((long long)mj * (long long)mk)) % mi;
   j = (n / mk) % mj;
   k = n % mk;
   calc_vars();
}

void ThreeSequence::calc_vars()
{
   firsti = i == 0;
   firstj = j == 0;
   firstk = k == 0;
   halfi = 2 * i == mi - (mi % 2);
   halfj = 2 * j == mj - (mj % 2);
   halfk = 2 * k == mk - (mk % 2);
   lasti = i == (mi - 1);
   lastj = j == (mj - 1);
   lastk = k == (mk - 1);

   if (firstflag)
   {
      firstflag = false;
      loopend = false;
   }
   else
   {
      loopend = firsti && firstj && firstk;
   }

   i += offi;
   j += offj;
   k += offk;
}

void ThreeSequence::show()
{
   printf("%3lld ijk %2d %2d %2d flagi %1d %1d %1d flagj %1d %1d %1d flagk %1d %1d %1d\n", 
      n, i, j, k,
      firsti, halfi, lasti,
      firstj, halfj, lastj,
      firstk, halfk, lastk);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace