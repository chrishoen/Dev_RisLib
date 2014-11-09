/*==============================================================================
Description:
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include <math.h>
#include <string.h>

#include "risLUT.h"

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Lut1D::initialize(
      int     aRows,
      double  aFirstX, 
      double  aDeltaX,
      double* aVTable)
{
   mRows   = aRows;
   mFirstX = aFirstX;
   mLastX  = aFirstX + aDeltaX*(aRows-1);
   mDeltaX = aDeltaX;
   mVTable = aVTable;

   Prn::print(0,0,"mRows       %4d",mRows);
   Prn::print(0,0,"mFirstX   %6.2f",mFirstX);
   Prn::print(0,0,"mLastX    %6.2f",mLastX);
   Prn::print(0,0,"mDeltaX   %6.2f",mDeltaX);
   Prn::print(0,0,"");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

double Lut1D::lookup (double aX)
{
   int tRow;
   double tX1,tX2,tV1,tV2,tX,tV,tM;

   if (aX < mFirstX) return mVTable[0];
   if (aX > mLastX)  return mVTable[mRows-1];

   tX  = aX - mFirstX;
   tRow = (int)floor(tX/mDeltaX);
   tX1 = (tRow)*mDeltaX;
   tX2 = (tRow+1)*mDeltaX;
   tV1 = mVTable[tRow];
   tV2 = mVTable[tRow+1];
   tM  = (tV2 - tV1)/(tX2 - tX1);
   tV  = tV1 + tM*(tX - tX1);

   Prn::print(0,0,"aX        %6.2f",aX);
   Prn::print(0,0,"tRow      %4d",  tRow);
   Prn::print(0,0,"tX1       %6.2f",tX1);
   Prn::print(0,0,"tX        %6.2f",tX);
   Prn::print(0,0,"tX2       %6.2f",tX2);
   Prn::print(0,0,"tV1       %6.2f",tV1);
   Prn::print(0,0,"tV        %6.2f",tV);
   Prn::print(0,0,"tV2       %6.2f",tV2);

   return tV;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

void Lut2D::initialize(
   int     aRows,
   int     aCols,
   double  aFirstX, 
   double  aFirstY, 
   double  aDeltaX,
   double  aDeltaY,
   double* aVTable)
{
   mRows   = aRows;
   mCols   = aCols;

   mFirstX = aFirstX;
   mLastX  = aFirstX + aDeltaX*(aRows-1);
   mDeltaX = aDeltaX;

   mFirstY = aFirstY;
   mLastY  = aFirstY + aDeltaY*(aCols-1);
   mDeltaY = aDeltaY;

   mVTable = aVTable;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

double Lut2D::lookup(int aRow,int aCol)
{
   return *(mVTable + aRow*mCols + aCol);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************

double Lut2D::lookup (double aX,double aY)
{
   //---------------------------------------------------------------------------
   // Locals

   int tRow,tCol;
   double tX1,tX2,tX;
   double tY1,tY2,tY;
   double tV,tVnumer,tVdenom;
   double tF11,tF21,tF12,tF22;

   //---------------------------------------------------------------------------
   // Calculate X

   if (aX < mFirstX)
   {
      tRow = 0;
      tX   = 0.0;
   }
   else if (aX > mLastX)
   {
      tRow = mRows-1;
      tX   = mLastX - mFirstX;
   }
   else
   {
      tX  = aX - mFirstX;
      tRow = (int)floor(tX/mDeltaX);
   }
   tX1 = (tRow)*mDeltaX;
   tX2 = (tRow+1)*mDeltaX;

   //---------------------------------------------------------------------------
   // Calculate Y

   if (aY < mFirstY)
   {
      tCol = 0;
      tY   = 0.0;
   }
   else if (aY > mLastY)
   {
      tCol = mCols-1;
      tY   = mLastY - mFirstY;
   }
   else
   {
      tY  = aY - mFirstY;
      tCol = (int)floor(tY/mDeltaY);
   }
   tY1 = (tCol)*mDeltaY;
   tY2 = (tCol+1)*mDeltaY;


   //---------------------------------------------------------------------------
   // Calculate V

   tF11 = lookup(tRow  , tCol  );
   tF21 = lookup(tRow+1, tCol  );
   tF12 = lookup(tRow  , tCol+1);
   tF22 = lookup(tRow+1, tCol+1);

   tVnumer = 
      (tF11*(tX2 - tX )*(tY2 - tY ) +
       tF21*(tX  - tX1)*(tY2 - tY ) + 
       tF12*(tX2 - tX )*(tY  - tY1) + 
       tF22*(tX  - tX1)*(tY  - tY1)); 

   tVdenom = (tX2 - tX1)*(tY2 - tY1);

   tV = tVnumer/tVdenom;

   Prn::print(0,0,"aX        %6.2f",aX);
   Prn::print(0,0,"aY        %6.2f",aY);
   Prn::print(0,0,"tRow      %4d",  tRow);
   Prn::print(0,0,"tCol      %4d",  tCol);
   Prn::print(0,0,"tX1       %6.2f",tX1);
   Prn::print(0,0,"tX        %6.2f",tX);
   Prn::print(0,0,"tX2       %6.2f",tX2);
   Prn::print(0,0,"tY1       %6.2f",tY1);
   Prn::print(0,0,"tY        %6.2f",tY);
   Prn::print(0,0,"tY2       %6.2f",tY2);
   Prn::print(0,0,"tV        %6.2f",tV);

   return tV;
}

}//namespace

