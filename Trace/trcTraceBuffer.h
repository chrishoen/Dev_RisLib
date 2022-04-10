#pragma once

/*==============================================================================
Program trace buffer facility. 
==============================================================================*/

//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "risThreadsSynch.h"
#include "risCmdLineCmd.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Trc
{

//******************************************************************************
//******************************************************************************
//******************************************************************************
// This is a class that establishes a program trace facility that manages
// the storing and displaying of program trace debug print strings. It
// contains four pairs of trace buffers. Each pair contains a buffer that
// stores the first N strings in a trace and a circular buffer that contains
// the last N strings in a trace. Write access to the buffers are protected
// by mutex semaphores.

class TraceBuffer
{
public:

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Constants.

   // Number of available trace buffers.
   static const int cNumTraces = 100;

   // Number of string elements per buffer.
   static const int cNumStrings = 200;

   // Max string element size.
   static const int cMaxStringSize = 99;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Array of pointers to string buffers that stores the first N writes.
   // This is equivalent to:
   // char mBufferFirst[cNumTraces][cNumStrings][cMaxStringSize];
   char* mBufferFirst[cNumTraces];

   // Array of pointers to circular string buffers that stores the last N writes.
   // This is equivalent to:
   // char mBufferLast[cNumTraces][cNumStrings][cMaxStringSize];
   char* mBufferLast[cNumTraces];

   // True if a buffer pair has been created.
   bool mBufferExists[cNumTraces];

   // For each buffer pair, the index of the next string to write to. This is
   // incremented after each write and it increases indefinitely. The first
   // buffer is only written to for the first N strings and the last buffer
   // is written to modulo N, where N is the number of string elements
   // in a buffer.
   long long mNextWriteIndex[cNumTraces];

   // If true then writes are enabled for a buffer pair.
   bool mWriteEnable[cNumTraces];

   // If true then writes are suspended for a buffer pair.
   bool mWriteSuspend[cNumTraces];

   // True if a trace log file has been created.
   bool mLogExists[cNumTraces];

   // If true then logging is enabled. In this case all enabled
   // writes are written to a log file.
   bool mLogEnable[cNumTraces];

   // Trace write level. Write calls supply a level argument. If it is
   // greater than or equal to the write level then the write is executed.
   int mWriteLevel[cNumTraces];

   // Trace write level. Write calls supply a level argument. If it is
   // greater than or equal to the log level then the write is written
   // to a log file.
   int mLogLevel[cNumTraces];

   // Mutexes that protect all starts, stops, writes, and shows.
   Ris::Threads::MutexSemaphore mMutex[cNumTraces];

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Members.

   // Default command line arguments.
   int mDefaultTraceIndex;
   int mDefaultShowSize;

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Infrastucture.

   // Constructor,
   TraceBuffer();
   void reset();
   void initialize();
   void finalize();

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Write.

   // Allocate memory for a trace buffer pair. Set the initial write level
   // for the trace.
   void doCreateBuffer(int aTraceIndex, int aWriteLevel);

   // Disable a trace and deallocate its memory.
   void doDeleteBuffer(int aTraceIndex);

   // Create a log file for a trace. Set the initial log level
   // for the trace.
   void doCreateLogFile(int aTraceIndex, int aLogLevel, const char* aFilePath);

   // Start a trace on a buffer pair. Reset the write index and enable writes.
   // If the buffer number is -1 then start all buffers.
   void doStart(int aTraceIndex);

   // Stop a trace on a buffer pair. Disable writes.
   // If the buffer number is -1 then stop all buffers.
   void doStop(int aTraceIndex);

   // Suspend a trace on a buffer pair.
   void doSuspend(int aTraceIndex);

   // Resume a suspended trace on a buffer pair.
   void doResume(int aTraceIndex);

   // If enabled and not suspended, write a string to a buffer pair at the
   // write index and advance the write index. For the first buffer of the
   // pair this only writes the first N strings. For the last buffer of the
   // pair this writes circulary modulo N. If writes are enabled and logs
   // are enabled then this writes the string to a log file.
   void doWrite(int aTraceIndex, int aLevel, const char* aString);

   // Suspend tracing, show a first or last buffer, and resume tracing.
   void doShowFirst(int aTraceIndex, int aNumStrings);
   void doShowLast(int aTraceIndex, int aNumStrings);

   // Show trace buffer status.
   void doShowStatus();

   // Execute a command line command to manage and show the trace buffers.
   void execute(Ris::CmdLineCmd* aCmd);

   //***************************************************************************
   //***************************************************************************
   //***************************************************************************
   // Methods. Helpers.

   // Return true if a trace buffer has been created.
   bool isValidTrace(int aTraceIndex);

   // Return true if a trace buffer log file has been created.
   bool isValidLog(int aTraceIndex);

   // Return a pointer to a string element in a first buffer,
   // based on an index.
   char* stringAtFirst(int aTraceIndex, long long aStringIndex);

   // Return a pointer to a string element in a last buffer,
   // based on an index modulo the number of elements.
   char* stringAtLast(int aTraceIndex, long long aStringIndex);
};

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Global instance.

#ifdef _TRCTRACEBUFFER_CPP_
   TraceBuffer gTraceBuffer;
#else
   extern TraceBuffer gTraceBuffer;
#endif

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace