#ifndef _RISTIMEREP_H_
#define _RISTIMEREP_H_
/*==============================================================================
Nanosecond time representation and conversions.
Time representation is Int64, with LSB of 1 nanosecond
==============================================================================*/

#include <time.h>
#include "risPortableTypes.h"

//*****************************************************************************

namespace Ris
{
namespace NanoTime
{

   //---------------------------------------------------------------------------
   // Unit conversions

   Int64  getNsFromSec  (double aTimeSec);
   Int64  getNsFromMs   (int    aTimeMs);
   Int64  getNsFromUs   (int    aTimeUs);

   double getSecFromNs  (Int64  aTimeNs);
   int    getMsFromNs   (Int64  aTimeNs);
   int    getUsFromNs   (Int64  aTimeNs);
   
   //---------------------------------------------------------------------------
   // timespec conversions

   void getNsFromTimespec (Int64&  aTimeNs, void* aTimespec);
   void getTimespecFromNs (void* aTimespec, Int64 aTimeNs);

   //---------------------------------------------------------------------------
   // tm conversions

   void getNsFromTm (Int64&  aTimeNs, struct tm* aTm);
   void getTmFromNs (struct tm* aTm,  Int64      aTimeNs);

   //---------------------------------------------------------------------------
   // String conversions

   char* asString1(Int64 aTimeNs,char* aString);
   char* asString2(Int64 aTimeNs,char* aString);
   char* asString3(Int64 aTimeNs,char* aString);

}//namespace
}//namespace
#endif

