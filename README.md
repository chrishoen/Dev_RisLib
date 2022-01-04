# Dev_RisLib
### real time infrastructure library, C++, vstudio, cmake windows and linux

This is a library of C++ classes that provides some realtime functionality. Features include:
   1. Thread base classes.
   2. QCall threads. These are threads that service queues that contain calls (class function pointers).
   3. ByteContent, a scheme to encapsulate binary messages with C++ classes and pack/unpack them into byte buffers.
   4. Socket classes. Serial port classes.
   5. Threads for TCP server and client, UDP, and serial that are used to communicate binary messages.
   6. Command line executive classes for console and parameter and parameters files.
   7. A filtered print facility that can redirect to separate windows.
   8. Thread Services classes that provide for instumentation of threads.
   8. Miscellaneous utility functions.

This library is written for Windows and Linux in Visual Studio, using the cmake paradigm. The classes are based on Win32 or pthread realtime functions for threads and sockets and such. The library is portable. The details of the provided functionality that are specific to Win32 are contained in a small set of `*_win.cpp` or `*_linux.cpp`files. It is the intention of the library that programs built under Windows can be rebuilt seamlessly under Linux. CMake is used to implement the cross platform functionality.

The solution contains the library project and a set of associated unit test projects.

## Classes
``` markdown
logFiles.h                    -- log file facility
my_defs.h                     -- some #defines
my_functions.h                -- some string, index, time utilities
prnPrint.h                    -- filtered print facility
prnPrintFilter.h              -- filtered print facility, filter constants
prnPrintFilterTable.h         -- filtered print facility, filter table
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
Classes         -- constructor and destructor experiments
CmdLineTest1    -- experiments for command line commands
CommonLib       -- common library -- byte content messages and settings
Experiment      -- experiments
Functions       -- function pointer experiments
Parms           -- unit test for parms files
PrintTest       -- unit test for print facility
PrintView       -- PrintView.exe build
Program         -- unit test for program configuration
ProtoSerial     -- prototype for byte content message, serial
ProtoString     -- prototype for byte content message, upd string
ProtoTcpClient  -- prototype for byte content message, tcp client
ProtoTcpServer  -- prototype for byte content message, tcp client
ProtoUdp        -- prototype for byte content message, udp peer
RisLib          -- realtime infrastructure library -- this is the library
SerialComm      -- unit test for serial communications
TableTest       -- unit est for lookup tables
Test            -- general test project
ThreadNotify    -- unit test for thread notification class
ThreadQCall     -- unit test for qcall threads
ThreadSample    -- sample thread project
ThreadServices  -- unit test for thread services classes
ThreadSynch     -- unit test for thread synchronization classes
ThreadTest      -- unit test for thread base classes
TimerTest       -- unit test for timer classes
TimerThreadTest -- unit test for timer threads and priorites and cores
```
 
## Build target platforms
``` markdown
This supports builds for the following platforms:

beagle  -- builds on windows for a debian arm7 target, tested on beaglebone black, raspberrypi4
dart    -- builds on remote ubuntu for a yocto aarch64 target, tested on veriscite dart
ubuntu  -- builds on remote ubuntu for a ubuntu target, tested on intel nuc
windows -- builds on windows for a windows target, tested on intel nuc
```

## CMake Files
``` markdown
CMakeLists.txt               -- Solution main cmake file
CMakeSettings.json           -- current  cmake settings, copied from one of the following
CMakeSettings_beagle.json    -- specific cmake settings
CMakeSettings_dart.json      -- specific cmake settings
CMakeSettings_ubuntu.json    -- specific cmake settings
CMakeSettings_win.json       -- specific cmake settings
imx8mm-toolchain.cmake       -- specific toolchain file
MyBeagleToolchain.cmake      -- specific toolchain file
MyCodeMacros.cmake           -- macros for compiler settings
MyConfig.cmake               -- current  config variables, copied from one of the following
MyConfig_beagle.cmake        -- specific config variables
MyConfig_dart.cmake          -- specific config variables
MyConfig_ubuntu.cmake        -- specific config variables   
MyConfig_win.cmake           -- specific config variables
MyGlobalImports.cmake        -- macros for importing global libraries
MyLocalImports.cmake         -- macros for importing local libraries
MyOverrides.cmake            -- some cmake overrides
MyPostBuild.cmake            -- macros for post build copying
MyPrecompiledHeader.cmake    -- macros for using precompiled headers
MySpecial.cmake              -- special debuf stuff  
set_for_beagle.bat           -- copy specific CMakeSettings.json and MyConfig.cmake to current
set_for_dart.bat             -- copy specific CMakeSettings.json and MyConfig.cmake to current
set_for_ubuntu.bat           -- copy specific CMakeSettings.json and MyConfig.cmake to current
set_for_win.bat              -- copy specific CMakeSettings.json and MyConfig.cmake to current

Before start a new build session, run one of the set_for_* bat files
```
