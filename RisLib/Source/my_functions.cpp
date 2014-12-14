/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "my_functions.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Min,Max

double my_fmin (double aA,double aB) {return aA > aB ? aB : aA;}
double my_fmax (double aA,double aB) {return aA > aB ? aA : aB;}

int    my_imin (int aA,int aB) {return aA > aB ? aB : aA;}
int    my_imax (int aA,int aB) {return aA > aB ? aA : aB;}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Close to

bool my_closeto (double aA,double aB,double aResolution)
{
   return (fabs(aA - aB) <= aResolution);
}

bool my_closeto (double aA,double aB,int aResolution)
{
   switch (aResolution)
   {
      case  6  : return (fabs(aA - aB) <= 1000000.0);
      case  5  : return (fabs(aA - aB) <= 100000.0);
      case  4  : return (fabs(aA - aB) <= 10000.0);
      case  3  : return (fabs(aA - aB) <= 1000.0);
      case  2  : return (fabs(aA - aB) <= 100.0);
      case  1  : return (fabs(aA - aB) <= 10.0);
      case  0  : return (fabs(aA - aB) <= 1.0);
      case -1  : return (fabs(aA - aB) <= 0.1);
      case -2  : return (fabs(aA - aB) <= 0.01);
      case -3  : return (fabs(aA - aB) <= 0.001);
      case -4  : return (fabs(aA - aB) <= 0.0001);
      case -5  : return (fabs(aA - aB) <= 0.00001);
      case -6  : return (fabs(aA - aB) <= 0.000001);
      case -7  : return (fabs(aA - aB) <= 0.0000001);
      case -8  : return (fabs(aA - aB) <= 0.00000001);
      default  : return (fabs(aA - aB) <= 0.01);
   }
   return false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Round to nearest integer

int my_round (double aA)
{
   if   (aA >= 0.0)  return int(aA + 0.5);
   else              return int(aA - 0.5);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Random numbers

// Seeds random with current time
void my_srand()
{
   unsigned int tTime = ((unsigned int)time(NULL))&0x7fff;
   srand(tTime*117);
}

// Return random double 0.0 <= x < 1.0
double my_frand()
{
   return double(rand())/RAND_MAX;
}

// Return random double A <= x < B
double my_frand(double aA,double aB)
{
   double tR=my_frand();
   return aA + (aB-aA)*tR; 
}

// Return random int 0 <= x < A
int my_irand(int aA)
{
   return rand() % aA;
}

// Return random int A <= x <= B
int my_irand(int aA,int aB)
{
   return aA + rand() % (aB - aA + 1);
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Strings

// Trim CRLF from the end of a string
void my_trimCRLF(char* aString)
{
   // Remove cr/lf at end of line
   unsigned tStringLen = strlen(aString);
   if (aString[tStringLen-1]==0xa)
   {
      aString[tStringLen-1]=0;

      if (aString[tStringLen-2]==0xd)
      {
         aString[tStringLen-2]=0;
      }
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Time conversions

double my_time_from_string(char* aString)
{
   int tLength = strlen(aString);
   char tString[40];
   double tTime=0.0;

   switch (aString[tLength-1])
   {
   case 's' :
   case 'S' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      break;
   case 'm' :
   case 'M' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      tTime *= 60.0;
      break;
   case 'h' :
   case 'H' :
      strncpy(tString,aString,tLength-1);
      tTime = atof(tString);
      tTime *= 3600.0;
      break;
   default:
      tTime = atof(aString);
      break;
   }
   return tTime;
}

char*  my_string_from_time(double aTime, char* aBuffer)
{
   int     tTimeSeconds  = (int)aTime;
// double  tTimeSecondsF = aTime;

   int  tSecInOneMin   = 60;
   int  tSecInOneHour  = 60*60;
   int  tSecInOneDay   = 60*60*24;

   int  tSecInDay  = tTimeSeconds  % tSecInOneDay;
   int  tSecInHour = tTimeSeconds  % tSecInOneHour;
   int  tSecInMin  = tTimeSeconds  % tSecInOneMin;

   int  tHourInDay = tSecInDay  / tSecInOneHour;
   int  tMinInHour = tSecInHour / tSecInOneMin;

   sprintf(aBuffer,"%02d:%02d:%02d",tHourInDay,tMinInHour,tSecInMin);
   return aBuffer;
}
