#ifndef _RISFILTERFILTERS_H_
#define _RISFILTERFILTERS_H_
/*==============================================================================

filters
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{
namespace Filter
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
   enum { MaxFilterSize = 200 };

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Sliding window average, calculates expectation (mean)
// and uncertainty (standard deviation)

class MovingAverage
{
public:
   // Initialize
   void initialize(int aSize); 
   void reset(); 
   void show();
   // Valid
   bool mValid;

   // Put input value
   void put(double aX);

   // Input value
   double mX;
   // Output expectation (mean)
   double mE;
   // Output uncertainty (standard deviation)
   double mU;

   // Members
   int mSize;
   int mCount;
   int mIndex;
   int mK;

   double mEArray[MaxFilterSize];
   double mESum;
   double mUArray[MaxFilterSize];
   double mUSum;
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Shift register of doubles, it shifts to the right, the left end element
// is the most recent and the right end element is the latest.

class ShiftRegister
{
public:
   // Initialize
   void initialize(int aSize);
   void reset(); 
   void show();
   // Valid
   bool mValid;

   // Shift right and put input value at the left end.
   void shiftRight(double aX);

   // Set the left end value
   void setLeft(double aX);

   // Input value
   double mX;

   // Get value
   double get(int    aIndex);
   double getLeft();
   double getRight();

   // Members

   // Array size
   int mSize;
   // Index of left end
   int mIndex;
   // Number of occupied elements
   int mCount;
   // Number of shifts
   int mK;

   // Value array
   enum { MaxShiftRegisterSize = 1000 };
   double mArray[MaxShiftRegisterSize];
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// FIRFilter

class FIRFilter
{
public:
   // Initialize
   void initialize(int aSize); 
   void reset(); 
   void show();
   // Valid
   bool mValid;

   // Put input value
   double put(double aX);

   // Input value
   double mX;

   // Output value
   double mY;

   // Value array
   ShiftRegister mXSR;

   // Coefficient array
   double mBArray[MaxFilterSize];
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// FIRTestFilter

class FIRTestFilter : public FIRFilter
{
public:
   // Initialize
   void initialize(); 
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// IIRFilter

class IIRFilter
{
public:
   // Initialize
   void initialize(int aYSize,int aXSize); 
   void reset(); 
   void show();
   // Valid
   bool mValid;

   // Put input value
   double put(double aX);

   // Input value
   double mX;

   // Output value
   double mY;

   // Value arrays
   ShiftRegister mYSR;
   ShiftRegister mXSR;

   // Coefficient array
   double mAArray[MaxFilterSize];
   double mBArray[MaxFilterSize];
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// IIRTestFilter

class IIRTestFilter : public IIRFilter
{
public:
   // Initialize
   void initialize(); 
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// MovingDerivative

class MovingDerivative : public FIRFilter
{
public:
   // Initialize
   void initialize(int aOrder,double aDeltaTime); 
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This filter characterizes an input signal by calculating expectations and
// uncertainties of the signal and its time derivative.

class CharacterFilter1
{
public:
   // Initialize
   void initialize(double aDeltaTime); 
   void reset(); 
   void show();
   // Valid
   bool mValid;

   // Put input value
   void put(double aX);

   // Output values
   double mX;       // Input value
   double mV;       // Derivative of input 
   double mEX;      // Input expectation
   double mUX;      // Input uncertainty
   double mEV;      // Derivative expectation
   double mUV;      // Derivative uncertainty

   // Averages and derivatives
   MovingAverage    mXMA;   // Moving average of input
   MovingDerivative mXMD;   // Moving derivative of input
   MovingAverage    mVMA;   // Moving average of derivative of input

   // Members
   int mK;
};

}//namespace
}//namespace
#endif


