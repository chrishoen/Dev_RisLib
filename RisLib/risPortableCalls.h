#pragma once

/*==============================================================================
These calls are intended to be portable across different
software development environments and operating systems.
==============================================================================*/
// git test 101
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
char* portableGetBinDir();
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
  
// Fill the input buffer with a full file path, removing relative
// references (../../ etc). If the input buffer is null then
// create one. Also return the filled buffer.
char* portableFullPath(char* aBuffer, const char* aFilePath);

// Set the program current working directory.
void portableChdir(const char* aFilePath);

// Set the program current working directory up one level from the 
// program bin directory.
void portableChdirUpFromBin();

// Set the program current working directory to the program bin
// directory.
void portableChdirToBin();

// Return the current time as a string.
char* portableGetCurrentTimeAsString(char* aBuffer, bool aSecondDecimals = false);

// Return true if running windows, false if running linux.
bool portableIsWindows();

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace


