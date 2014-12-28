#ifndef _RISTIMEMARKER_H_
#define _RISTIMEMARKER_H_
/*==============================================================================

This file contains a class that encapsulate a time marker that can be used
to measure computer execution time durations

==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************
#include <time.h>
#include "risPortableTypes.h"
#include "risByteContent.h"


namespace Ris
{

//******************************************************************************

class  TimeMarker : public Ris::ByteContent
{
public:
   // -------------------------------------
   TimeMarker();
   void copyToFrom (Ris::ByteBuffer* aBuffer);

   void doBegin();
   void doEnd();
   
   double elapsedTimeInSeconds();
   double elapsedTimeInMilliseconds();
   double elapsedTimeInMicroseconds();
   // -------------------------------------
   double  mDelta;
   char    mString[20];
   char    mStr[20];


   char* asString1();
   char* asString2();


   //------------------------------------------------
   // Content

   long long mBegin;
   long long mEnd;
   double     mPerformanceFrequency;

   // Content
   //------------------------------------------------
};
}//namespace

#endif
