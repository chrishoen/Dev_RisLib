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
   calc_flags();
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
   calc_flags();
}

void ThreeSequence::calc_flags()
{
   first_i = i == 0;
   first_j = j == 0;
   first_k = k == 0;
   half_i = i == mi / 2;
   half_j = j == mj / 2;
   half_k = k == mk / 2;
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

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace