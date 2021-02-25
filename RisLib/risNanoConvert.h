#pragma once

/*==============================================================================
Nanosecond time representation and conversions.
Time representation is long long, with LSB of 1 nanosecond
==============================================================================*/

#include <time.h>

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace NanoConvert
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are functions that can be used to convert between a long long
// nanosecond time representation (where the lsb is one nanosecond) and
// various other time representations.

//------------------------------------------------------------------------------
// Unit conversions

long long  getNsFromSec  (double aTimeSec);
long long  getNsFromMs   (int    aTimeMs);
long long  getNsFromUs   (int    aTimeUs);

double getSecFromNs  (long long  aTimeNs);
int    getMsFromNs   (long long  aTimeNs);
int    getUsFromNs   (long long  aTimeNs);
   
//------------------------------------------------------------------------------
// timespec conversions

long long getNsFromTimespec (struct timespec* aTimespec);
struct timespec  getTimespecFromNs (long long aTimeNs);

//------------------------------------------------------------------------------
// tm conversions

void getNsFromTm (long long&  aTimeNs, struct tm* aTm);
void getTmFromNs (struct tm* aTm,  long long      aTimeNs);


//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace
}//namespace

