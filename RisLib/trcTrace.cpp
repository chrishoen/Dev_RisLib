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

// Allocate memory for a trace buffer. Set the initial write level.
void create_buffer(int aTraceIndex, int aWriteLevel, const char* aLabel)
{
   gTraceBuffer.doCreateBuffer(aTraceIndex, aWriteLevel, aLabel);
}

// Create a log file for a trace. Set the initial log level
// for the trace.
void create_log(int aTraceIndex, int aLogLevel, const char* aFilePath, bool aAppend)
{
   gTraceBuffer.doCreateLogFile(aTraceIndex, aLogLevel, aFilePath, aAppend);
}

// Create a log file for a trace. Set the initial log level
// for the trace.
void create_log(int aTraceIndex, int aLogLevel, std::unique_ptr<std::string> aFilePath, bool aAppend)
{
   gTraceBuffer.doCreateLogFile(aTraceIndex, aLogLevel, aFilePath->c_str(), aAppend);
}

// Initialize.
void initialize()
{
   gTraceBuffer.initialize();
}

// Finalize.
void finalize()
{
   gTraceBuffer.finalize();
}

// Initialize.
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

// Suspend a trace on a buffer.
void suspend(int aTraceIndex)
{
   gTraceBuffer.doSuspend(aTraceIndex);
}

// Resume a suspended trace on a buffer.
void resume(int aTraceIndex)
{
   gTraceBuffer.doResume(aTraceIndex);
}

// Set default show trace index.
void set_default_trace_index(int aTraceIndex)
{
   gTraceBuffer.mDefaultTraceIndex = aTraceIndex;
}


//******************************************************************************
//******************************************************************************
//******************************************************************************
// Write to a trace buffer.

void write (int aTraceIndex, int aLevel, const char* aFormat, ...)
{
   if (!gTraceBuffer.isWritable(aTraceIndex, aLevel)) return;

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
// Use absolute or program time for timetag.

void use_absolute_timetag()
{
   gTraceBuffer.mUseAbsoluteTime = true;
}

void use_program_timetag()
{
   gTraceBuffer.mUseAbsoluteTime = false;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
} //namespace

