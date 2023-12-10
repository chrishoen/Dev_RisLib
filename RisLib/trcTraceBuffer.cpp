//******************************************************************************
//******************************************************************************
//******************************************************************************

#include "stdafx.h"

#include "my_functions.h"
#include "risProgramTime.h"
#include "risCmdLineFile.h"

#define  _TRCTRACEBUFFER_CPP_
#include "trcTraceBuffer.h"

//******************************************************************************
//******************************************************************************
//******************************************************************************

namespace Trc
{

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************
   // Constructor.

   TraceBuffer::TraceBuffer()
   {
      reset();
   }

   void TraceBuffer::reset()
   {
      for (int i = 0; i < cNumTraces; i++)
      {
         mBufferFirst[i] = 0;
         mBufferLast[i] = 0;
         mBufferExists[i] = false;
         mNextWriteIndex[i] = 0;
         mWriteEnable[i] = false;
         mWriteSuspend[i] = false;
         mLogExists[i] = false;
         mLogFile[i] = 0;
         mWriteLevel[i] = 0;
         mLogLevel[i] = 0;
         mMutex[i] = 0;
         mTraceLabel[i][0] = 0;
         mFirstTimetag = 0;
         mWriteFirstFlag = false;
         mUseAbsoluteTime = true;
      }
      mTraceIndexSet.clear();
      mInitFlag = false;
      mDefaultTraceIndex = 1;
      mDefaultShowSize = 40;
   }

   //******************************************************************************
   //******************************************************************************
   //******************************************************************************

   void TraceBuffer::initialize()
   {
      mInitFlag = true;
   }

   void TraceBuffer::finalize()
   {
      for (const int& tTraceIndex : mTraceIndexSet)
      {
         doStop(tTraceIndex);
         mMutex[tTraceIndex]->lock();
         free(mBufferFirst[tTraceIndex]);
         free(mBufferLast[tTraceIndex]);
         mBufferFirst[tTraceIndex] = 0;
         mBufferLast[tTraceIndex] = 0;
         mBufferExists[tTraceIndex] = false;
         mNextWriteIndex[tTraceIndex] = 0;
         mWriteEnable[tTraceIndex] = false;
         mWriteSuspend[tTraceIndex] = false;
         mWriteLevel[tTraceIndex] = 0;
         mLogLevel[tTraceIndex] = 0;
         mMutex[tTraceIndex]->unlock();
         delete mMutex[tTraceIndex];
         if (mLogExists[tTraceIndex])
         {
            fclose(mLogFile[tTraceIndex]);
         }
         mLogExists[tTraceIndex] = false;
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Allocate memory for a trace buffer pair. Set the initial write level
// for the trace.

void TraceBuffer::doCreateBuffer(int aTraceIndex, int aWriteLevel, const char* aLabel)
{
   // Guard.
   if (aTraceIndex < 1 || aTraceIndex >= cNumTraces) return;

   // Allocate memory for the trace buffer pair and set variables.
   mBufferFirst[aTraceIndex] = (char*)malloc(cNumStrings * (cMaxStringSize + 1));
   mBufferLast[aTraceIndex] = (char*)malloc(cNumStrings * (cMaxStringSize + 1));
   mBufferExists[aTraceIndex] = true;
   mWriteLevel[aTraceIndex] = aWriteLevel;
   mWriteEnable[aTraceIndex] = false;
   mMutex[aTraceIndex] = new Ris::Threads::MutexSemaphore;
   mTraceIndexSet.insert(aTraceIndex);
   strncpy(&mTraceLabel[aTraceIndex][0], aLabel, cMaxLabelSize);
   mTraceLabel[aTraceIndex][cMaxLabelSize] = 0;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Create a log file for a trace. Set the initial log level
// for the trace.

void TraceBuffer::doCreateLogFile(
   int aTraceIndex, int aLogLevel, const char* aFilePath, bool aAppend)
{
   // Guard.
   if (aTraceIndex < 1 || aTraceIndex >= cNumTraces) return;

   // Create the log file.
   if (aAppend)
   {
      mLogFile[aTraceIndex] = fopen(aFilePath, "a");
   }
   else
   {
      mLogFile[aTraceIndex] = fopen(aFilePath, "w");
   }

   if (mLogFile[aTraceIndex] == 0)
   {
      printf("TRACE ERROR COULD NOT CREATE LOG FILE %s\n", aFilePath);
      return;
   }

   // Set log file variables..
   mLogLevel[aTraceIndex] = aLogLevel;
   mLogExists[aTraceIndex] = true;
}

// Create a log file for a trace. Set the initial log level
// for the trace.
void TraceBuffer::doCreateLogFile(
   int aTraceIndex, int aLogLevel, std::unique_ptr<std::string> aFilePath, bool aAppend)
{
   if (aFilePath == 0)
   {
      printf("TraceBuffer::doCreateLogFile NULL\n");
      return;
   }

   doCreateLogFile(aTraceIndex, aLogLevel, aFilePath->c_str(), aAppend);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Validity tests.

// Return true if a trace buffer has been created.
bool TraceBuffer::isValidTrace(int aTraceIndex)
{
   if (aTraceIndex < 1 || aTraceIndex >= cNumTraces) return false;
   return mBufferExists[aTraceIndex];
}

// Return true if a trace buffer log file has been created.
bool TraceBuffer::isValidLog(int aTraceIndex)
{
   if (aTraceIndex < 1 || aTraceIndex >= cNumTraces) return false;
   return mLogExists[aTraceIndex];
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Pointers.

// Return a pointer to a string element in a first buffer,
// based on an index.
char* TraceBuffer::stringAtFirst(int aTraceIndex, long long aStringIndex)
{
   aStringIndex = aStringIndex < cNumStrings ? aStringIndex : cNumStrings - 1;
   char* tPtr = mBufferFirst[aTraceIndex] + (cMaxStringSize + 1) * aStringIndex;
   return tPtr;
}

// Return a pointer to a string element in a last buffer,
// based on an index modulo the number of elements.
char* TraceBuffer::stringAtLast(int aTraceIndex, long long aStringIndex)
{
   aStringIndex %= cNumStrings;
   char* tPtr = mBufferLast[aTraceIndex] + (cMaxStringSize + 1) * aStringIndex;
   return tPtr;
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Start a trace on a buffer pair. Reset the write index and enable writes.
// If the buffer number is -1 then start all buffers. 

void TraceBuffer::doStart(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mWriteFirstFlag = true;
   mNextWriteIndex[aTraceIndex] = 0;
   mWriteEnable[aTraceIndex] = true;
   mWriteSuspend[aTraceIndex] = false;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop a trace on a buffer pair. Disable writes.
// If the buffer number is -1 then stop all buffers.

void TraceBuffer::doStop(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mWriteEnable[aTraceIndex] = false;
   mWriteSuspend[aTraceIndex] = false;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume a stopped trace on a buffer pair. Enable writes. Don't change
// the write index. If the buffer number is -1 then resume all buffers. 

void TraceBuffer::doSuspend(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mWriteSuspend[aTraceIndex] = true;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Resume a stopped trace on a buffer pair. Enable writes. Don't change
// the write index. If the buffer number is -1 then resume all buffers. 

void TraceBuffer::doResume(int aTraceIndex)
{
   if (!isValidTrace(aTraceIndex)) return;
   mMutex[aTraceIndex]->lock();
   mWriteSuspend[aTraceIndex] = false;
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Return true if a trace buffer is writable.

bool TraceBuffer::isBufferWritable(int aTraceIndex, int aLevel)
{
   if (!isValidTrace(aTraceIndex)) return false;

   // Determine whether or not to write to the buffers.
   return
      mWriteEnable[aTraceIndex] &&
      !mWriteSuspend[aTraceIndex] &&
      aLevel <= mWriteLevel[aTraceIndex];
}

bool TraceBuffer::isLogWritable(int aTraceIndex, int aLevel)
{
   if (!isValidTrace(aTraceIndex)) return false;

   // Determine whether or not to write to the log file.
   return
      mWriteEnable[aTraceIndex] &&
      mLogExists[aTraceIndex] &&
      aLevel <= mLogLevel[aTraceIndex];
}

bool TraceBuffer::isWritable(int aTraceIndex, int aLevel)
{
   return
      isBufferWritable(aTraceIndex, aLevel) ||
      isLogWritable(aTraceIndex, aLevel);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// If enabled, write a string to a buffer pair at the write index and
// advance the write index. For the first buffer of the pair this only 
// write the first N strings. For the last buffer of the pair this writes
// circulary modulo N.

void TraceBuffer::doWrite(int aTraceIndex, int aLevel, const char* aString)
{
   // Guard.
   if (!isWritable(aTraceIndex, aLevel)) return;
   bool tWriteToBuffer = isBufferWritable(aTraceIndex, aLevel);
   bool tWriteToLog = isLogWritable(aTraceIndex, aLevel);

   // Lock.
   mMutex[aTraceIndex]->lock();

   // Get a timetag string.
   char tTimetagString[40];
   if (mUseAbsoluteTime)
   {
      my_timestamp4(tTimetagString);
      strcat(tTimetagString, " $ ");
   }
   else
   {
      // Get a timetag from the program time.
      double tTimetag = Ris::getProgramTime();
      sprintf(tTimetagString, "%7.3f $ ", tTimetag);

      // Store the first timetag.
      if (mWriteFirstFlag)
      {
         mWriteFirstFlag = false;
         mFirstTimetag = tTimetag;
      }
   }


   // 
   // Add the timetag and the input string to a temp string.
   char tString[cMaxStringSize + 1];
   strcpy(tString, tTimetagString);
   strcat(tString, aString);

   // Write to the buffers.
   if (tWriteToBuffer)
   {
      // Get buffer destination pointers,  they are determined by the
      // write index.
      char* tDestinFirst = 
         stringAtFirst(aTraceIndex, mNextWriteIndex[aTraceIndex]);
      char* tDestinLast = 
         stringAtLast(aTraceIndex, mNextWriteIndex[aTraceIndex]);

      // Copy to buffer.
      if (mNextWriteIndex[aTraceIndex] < cNumStrings)
      {
         strncpy(tDestinFirst, tString, cMaxStringSize);
         tDestinFirst[cMaxStringSize] = 0;
      }

      // Copy to buffer.
      strncpy(tDestinLast, tString, cMaxStringSize);
      tDestinLast[cMaxStringSize] = 0;

      // Advance the write index.
      mNextWriteIndex[aTraceIndex]++;
   }

   // Write to the log file.
   if (tWriteToLog)
   {
      // Append a cr to the string.
      strncat(tString, "\n", cMaxStringSize);
      tString[cMaxStringSize] = 0;
      // Write the string to the log file.
      fputs(tString, mLogFile[aTraceIndex]);
      // flush the write.
      fflush(mLogFile[aTraceIndex]);
   }

   // Unlock.
   mMutex[aTraceIndex]->unlock();
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop tracing and show a first buffer.

void TraceBuffer::doShowFirst(int aTraceIndex, int aShowSize)
{
   if (!isValidTrace(aTraceIndex)) return;
   printf("TRACE FIRST****************************** %d %d\n",aTraceIndex, aShowSize);
   if (mNextWriteIndex[aTraceIndex] == 0)
   {
      printf("EMPTY\n");
      return;
   }
   if (aShowSize > cNumStrings) aShowSize = cNumStrings;
   long long tNextWriteIndex = mNextWriteIndex[aTraceIndex];
   long long tShowSize = (long long)aShowSize;
   long long tStartIndex = 0;
   long long tLoopSize = 0;
   if (tNextWriteIndex > tShowSize)
   {
      tLoopSize = tShowSize;
      tStartIndex = 0;
   }
   else
   {
      tLoopSize = tNextWriteIndex;
      tStartIndex = 0;
   }
   for (long long i = 0; i < tLoopSize; i++)
   {
      long long tReadIndex = tStartIndex + i;
      printf("%4lld $ %s\n", tReadIndex, stringAtFirst(aTraceIndex, tReadIndex));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Stop tracing and show a last buffer.

void TraceBuffer::doShowLast(int aTraceIndex, int aShowSize)
{
   if (!isValidTrace(aTraceIndex)) return;
   if (mUseAbsoluteTime)
   {
      printf("TRACE LAST******************************* %d %d\n",
         aTraceIndex,
         aShowSize);
   }
   else
   {
      printf("TRACE LAST******************************* %d %d %.3f %.3f\n",
         aTraceIndex,
         aShowSize,
         mFirstTimetag,
         Ris::getProgramTime());
   }

   if (mNextWriteIndex[aTraceIndex] == 0)
   {
      printf("EMPTY\n");
      return;
   }
   if (aShowSize > cNumStrings) aShowSize = cNumStrings;
   long long tNextWriteIndex = mNextWriteIndex[aTraceIndex];
   long long tShowSize = (long long)aShowSize;
   long long tStartIndex = 0;
   long long tLoopSize = 0;
   if (tNextWriteIndex > tShowSize)
   {
      tLoopSize = tShowSize;
      tStartIndex = tNextWriteIndex - tLoopSize;
   }
   else
   {
      tLoopSize = tNextWriteIndex;
      tStartIndex = tNextWriteIndex - tLoopSize;
   }
   for (long long i = 0; i < tLoopSize; i++)
   {
      long long tReadIndex = tStartIndex + i;
      printf("%4lld $ %s\n", tReadIndex, stringAtLast(aTraceIndex, tReadIndex));
   }
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show trace buffer status.

void TraceBuffer::doShowStatus()
{
   printf("TRACE STATUS*****************************\n");
   printf("trace  trace               write  write log     log    write\n");
   printf("index  label               enable level exists  level  index\n");
   printf("\n");

   for (const int& i : mTraceIndexSet)
   {
      printf("%-4d   %-19s %-5s  %-1d     %-5s    %-1d     %-5lld\n",
         i,
         &mTraceLabel[i][0],
         my_string_from_bool(mWriteEnable[i]),
         mWriteLevel[i],
         my_string_from_bool(mLogExists[i]),
         mLogLevel[i],
         mNextWriteIndex[i]);
   }

   printf("\n");
   printf("DefaultTraceIndex       %d\n", mDefaultTraceIndex);
   printf("DefaultShowSize         %d\n", mDefaultShowSize);
   printf("CurrentProgramTime      %.3f\n", Ris::getProgramTime());
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show trace buffer status.

void TraceBuffer::doShowHelp()
{
   printf("TRACE FACILITY***************************\n");
   printf("TRC SHOW \n");
   printf("TRC START    tindex              -- start   a trace\n");
   printf("TRC STOP     tindex              -- stop    a trace\n");
   printf("TRC SUSPEND  tindex              -- suspend a trace\n");
   printf("TRC RESUME   tindex              -- resume  a trace\n");
   printf("TRC F        tindex   showsize   -- show the first trace buffer\n");
   printf("TRC L        tindex   showsize   -- show the last  trace buffer\n");
   printf("TRC TI       tindex              -- set the default trace index \n");
   printf("TRC S        showsize            -- set the default show size\n");
   printf("TRC LEVEL    tindex   level      -- set the write level\n");
   printf("\n");
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Execute a command line command to manage and show the trace buffers.

void TraceBuffer::execute(Ris::CmdLineCmd* aCmd)
{
   aCmd->setArgDefault(2, mDefaultTraceIndex);
   aCmd->setArgDefault(3, mDefaultShowSize);
   int tTraceIndex = aCmd->argInt(2);
   int tShowSize = aCmd->argInt(3);
   int tLevel = aCmd->argInt(3);
   if (!mInitFlag)
   {
      printf("TRACE NOT INITIALIZED\n");
      return;
   }

   if (aCmd->isArgString(1, "SHOW"))
   {
      doShowStatus();
   }
   else if (aCmd->isArgString(1, "HELP"))
   {
      doShowHelp();
   }
   else if (aCmd->isArgString(1, "START"))
   {
      printf("TRACE START  %d\n", tTraceIndex);
      doStart(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "STOP"))
   {
      printf("TRACE STOP   %d\n", tTraceIndex);
      doStop(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "SUSPEND"))
   {
      printf("TRACE RESUME %d\n", tTraceIndex);
      doSuspend(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "RESUME"))
   {
      printf("TRACE RESUME %d\n", tTraceIndex);
      doResume(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "F"))
   {
      aCmd->setArgDefault(3, 20);
      doSuspend(tTraceIndex);
      doShowFirst(tTraceIndex, tShowSize);
      doResume(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "L"))
   {
      doSuspend(tTraceIndex);
      doShowLast(tTraceIndex, tShowSize);
      doResume(tTraceIndex);
   }
   else if (aCmd->isArgString(1, "TI"))
   {
      mDefaultTraceIndex = aCmd->argInt(2);
   }
   else if (aCmd->isArgString(1, "S"))
   {
      mDefaultShowSize = aCmd->argInt(2);
   }
   else if (aCmd->isArgString(1, "LEVEL"))
   {
      mWriteLevel[tTraceIndex] = tLevel;
   }
   else
   {
      printf("TRACE INVALID COMMAND\n");
   }
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
// Show.

//******************************************************************************
//******************************************************************************
//******************************************************************************
}//namespace