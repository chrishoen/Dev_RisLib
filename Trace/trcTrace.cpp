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
void start(int aTraceIndex)
{
   gTraceBuffer.doStart(aTraceIndex);
}

// Stop a trace on a buffer.
void stop(int aTraceIndex)
{
   gTraceBuffer.doStop(aTraceIndex);
}

// Resume a stopped trace on a buffer.
void resume(int aTraceIndex)
{
   gTraceBuffer.doResume(aTraceIndex);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to a trace buffer.

void write (int aTraceIndex, int aLevel, const char* aFormat, ...)
{
   // Guard.
   if (!gTraceBuffer.isValidTrace(aTraceIndex)) return;
   if (!gTraceBuffer.mWriteEnable[aTraceIndex]) return;
   if (gTraceBuffer.mWriteSuspend[aTraceIndex]) return;
   if (gTraceBuffer.mWriteLevel[aTraceIndex] > aLevel) return;

   // Do a vsprintf with variable arg list into print string.
   static const int cMaxStringSize = TraceBuffer::cMaxStringSize;
   char tString[cMaxStringSize + 1];
   va_list  ArgPtr;
   va_start(ArgPtr, aFormat);
   vsnprintf(tString, cMaxStringSize, aFormat, ArgPtr);
   va_end(ArgPtr);
   tString[cMaxStringSize] = 0;

   // Write the string.
   gTraceBuffer.doWrite(aTraceIndex, aLevel, tString);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

