#pragma once

/*==============================================================================
Time since program start in seconds.
==============================================================================*/

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************

namespace Ris
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Read current time and subtract program start time.

// Return the current program time in seconds.
double getProgramTime();

// Return the current program time in milliseconds.
double getProgramTimeMS();

// Return the current program time in microseconds.
double getProgramTimeUS();

// Return the current program time in nanoseconds.
long long int getProgramTimeNS();

// Return the current cpu time in milliseconds.
unsigned int getCpuTimeUIntMS();

// Return the current cpu time in milliseconds.
long long getCpuTimeLongLongMS();

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
}//namespace

