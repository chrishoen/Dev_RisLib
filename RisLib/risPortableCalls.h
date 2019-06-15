#pragma once

/*==============================================================================
These calls are intended to be portable across different
software development environments and operating systems.
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************

// Sleep. Ticks are in milliseconds.
void portableSleep(int aTicks);

// Print a string and then halt the program.
void portableHalt(const char* aString = 0);

// Return a pointer to the current working directory.
char* portableGetProgramDir();
char* portableGetCurrentDir();

// Wait for key pressed.
void  portableWaitForKbhit();
bool  portableKbhit(void);

// Get high resolution time, nanoseconds.
unsigned long long int portableGetHiResCounter();
unsigned long long int portableGetHiResFrequency();

// convert string to upper case.
void portableStrupr(char* aString);

// Return true if file exists.
bool portableFilePathExists (const char* aFilePath);
  
// Set the program current working directory.
void portableChdir(const char* aFilePath);

// Return the current time as a string.
char* portableGetCurrentTimeAsString(char* aBuffer, bool aSecondDecimals = false);

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


