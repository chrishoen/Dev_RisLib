//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include <stdarg.h>

#include "trcTraceBuffer.h"
#include "trcTrace.h"

namespace Trc
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// These are aliase functioins for the trace buffer code. Refer there
// for an explanation.


// Reset the trace facility.
void reset()
{
   gTraceBuffer.reset();
}

// Start a trace on a buffer.
void start(int aBufNum)
{
   gTraceBuffer.doStart(aBufNum);
}

// Stop a trace on a buffer.
void stop(int aBufNum)
{
   gTraceBuffer.doStop(aBufNum);
}

// Resume a stopped trace on a buffer.
void resume(int aBufNum)
{
   gTraceBuffer.doResume(aBufNum);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to a trace buffer.

void write (int aBufNum, const char* aFormat, ...)
{
   //-----------------------------------------------------
   // Do a vsprintf with variable arg list into print string

   static const int cMaxStringSize = TraceBuffer::cMaxStringSize;
   char tPrintString[cMaxStringSize + 1];

   int tPrintSize;
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   tPrintSize = vsnprintf(tPrintString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);
   tPrintString[cMaxStringSize] = 0;

   //-----------------------------------------------------
   // Print the string

   gTraceBuffer.doWrite(aBufNum, tPrintString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

