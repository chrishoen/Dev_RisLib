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
   initialize(1, 1, 1);
}

ThreeSequence::ThreeSequence(int ami, int amj, int amk)
{
   initialize(ami, amj, amk);
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

void ThreeSequence::restart()
{
   initialize(mi, mj, mk);
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
   first_i = i == 0;
   first_j = j == 0;
   first_k = k == 0;
   half_i = 2 * i == mi - (mi % 2);
   half_j = 2 * j == mj - (mj % 2);
   half_k = 2 * k == mk - (mk % 2);
   last_i = i == (mi - 1);
   last_j = j == (mj - 1);
   last_k = k == (mk - 1);

   if (firstflag)
   {
      firstflag = false;
      loopend = false;
   }
   else
   {
      loopend = first_i && first_j && first_k;
   }
}

void ThreeSequence::show()
{
   printf("%3lld ijk %2d %2d %2d flagi %1d %1d %1d flagj %1d %1d %1d flagk %1d %1d %1d\n", 
      n, i, j, k,
      first_i, half_i, last_i,
      first_j, half_j, last_j,
      first_k, half_k, last_k);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace