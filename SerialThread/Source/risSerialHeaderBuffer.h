#pragma once
/*==============================================================================
Serial header buffer
=============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This class encalsulates a buffer of bytes that is used to detect serial 
// communications message headers. It is a shift register of bytes, it shifts
// up. The top element is the oldest and the bottom element is the most recent.

class SerialHeaderBuffer
{
public:
   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   char    mX;         // Input value
   int     mSize;      // Array size
   int     mIndex;     // Index of bottom element
   int     mCount;     // Number of occupied elements
   bool    mValid;     // Valid
   char*   mArray;     // Value array

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastucture.

   // Constructors.
   SerialHeaderBuffer();
  ~SerialHeaderBuffer();
   void reset(); 

   // Allocate memory and initialize.
   void initialize(int aSize);
   // Initialize, but reuse memory.
   void reinitialize();
   // Deallocate memory.
   void finalize();
   // Show.
   void show();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods.

   // Shift right and put input value at the left end.
   void shiftUp(char aX);

   // Set the left end value
   void setBottom(char aX);

   // Get value
   char get(int    aIndex);
   char getTop();
   char getBottom();

};

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace

