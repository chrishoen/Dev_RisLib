# Dev_RisLib
### real time infrastructure library, C++, vstudio

## Classes
``` markdown
logFiles.h                    -- log file facility
my_defs.h                     -- some #defines
my_functions.h                -- some string, index, time utilities
prnPrint.h                    -- filtered print facility
prnPrintSettings.h            -- filtered print facility, settings
risAlphaDir.h                 -- relocatable Alpha directory for C:\Alpha\Bin, C:\Alpha\Settings, etc
risByteBuffer.h               -- byte buffer for byte content binary message scheme
risByteContent.h              -- byte content base class for binary message scheme
risCallPointer.h              -- class templates that manage function pointers for c++ classes
risCmdLineCmd.h               -- command line command. is executed by a command line executive
risCmdLineConsole.h           -- execute command lines from the console, using a command line executive
risCmdLineExec.h              -- command line executive base class
risCmdLineFile.h              -- execute command lines from a file, using a command line executive
risCmdLineParms.h             -- support for command line parameters files
risCmdLineString.h            -- execute command lines from a string, using a command line executive
risCmdLineTables.h            -- lookup tables that can be read from a command line file
risContainers.h               -- some legacy container class templates, queues, stacks ...
risContainers2.h              -- more of the same
risLFIndex.h                  -- lock free index
risLMPacketQueue.h            -- a packet queue that uses semaphores to lock concurrent access
risLogic.h                    -- some and or not logic support for masks
risMemoryPtr.h                -- memory pointer support
risNanoTime.h                 -- nanosecond time facility based on cpu clock
risNetPortDef.h               -- default network socket port definitions
risNetSessionStateList.h      -- session state list used by a tcp server to track tcp client connections
risNetTcpMsgClientThread.h    -- tcp client thread based on byte content binary messages
risNetTcpMsgServerThread.h    -- tcp server thread based on byte content binary messages
risNetTcpMsgSocket.h          -- tcp socket for byte content binary messages 
risNetUdpMsgSocket.h          -- udp socket for byte content binary messages 
risNetUdpMsgThread.h          -- udp peer thread based on byte content binary messages
risNetUdpStringSocket.h       -- udp thread that passes strings 
risPointerStack.h             -- pointer stack
risPortableCalls.h            -- some system calls that are portable
risPortableTypes.h            -- legacy
risProgramTime.h              -- provides time from start of program based on hires cpu clock
risSerialHeaderBuffer.h       -- serial header buffer used to detect binary message headers
risSerialMsgPort.h            -- serial port for byte content binary messags
risSerialMsgThread.h          -- serial thread based on byte content binary messages
risSerialPort.h               -- portable serial port
risSockets.h                  -- portable socket classes
risStatistics.h               -- statistics classes for mean, stddev, min,max
risSystemTime.h               -- system time classes
risThreadsProcess.h           -- support for process priority and clocking
risThreadsQCall.h             -- qcalls are queue function calls, deferred procedure calls
risThreadsQCallThread.h       -- qcall thread, waits on a queue of calls and executes them serially
risThreadsQueue.h             -- legacy thread synchonized queues
risThreadsSynch.h             -- thread synchronization classes, mutex semaphores, event semaphores, etc
risThreadsThreads.h           -- thread base classes
risThreadsTimer.h             -- thread timer for periodic threads
risThreadsTimerThread.h       -- timer thread base class
risThreadsTwoThread.h         -- two thread base class, for threads that send a command and wait for a notification
risTimeMarker.h               -- time marker used to measure software timing
ris_priorities.h              -- default thread priorities
stdafx.h                      -- used for precompiled header files
```

## Solution Projects
``` markdown
ByteContent     -- unit test for byte content
CallPointer1    -- experiments for call pointers
CallPointer2    -- experiments for call pointers
Classes         -- experiments for classes
CmdLineTest1    -- experiments for command line commands
ConsoleTest     -- unit test for command line console
ExampleMsg      -- example byte content message set
Experiment      -- experiments
Files           -- parms files
NanoTimeTest    -- nano second unit test
Parms           -- unit test for parms files
PrintTest       -- unit test for print facility
PrintView       -- PrintView.exe build
Program         -- unit test for program configuration
ProtoCommon     -- prototype for byte content message common
ProtoCommS      -- prototype for byte content message, tcp server
ProtoCommT      -- prototype for byte content message, tcp client
ProtoCommU      -- prototype for byte content message, udp peer
RegHelper       -- register helpers
RisLib          -- realtime infrastructure library
Scratch         -- temp project
SerialComm      -- unit test for serial communications
SerialThread    -- unit test for byte content serial messages
TableTest       -- unit est for lookup tables
Test            -- general test project
ThreadQCall     -- unit test for qcall threads
ThreadSample    -- sample thread project
ThreadSynch     -- experiments for thread synchronization
ThreadTest      -- experiments for threads
TimerThreadTest -- unit test for timer threads and priorites and cores
TwoThreadTest   -- unit test for two threads
```
 
