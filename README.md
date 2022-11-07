# Dev_RisLib
### RisLib real time infrastructure library, C++, vstudio and vscode, cmake, windows and linux

This is a library of C++ classes that provides some realtime functionality. Features include:
   1. Thread base classes.
   2. QCall threads. These are threads that service queues that contain calls (class function pointers).
   3. ByteContent, a scheme to encapsulate binary messages with C++ classes and pack/unpack them into byte buffers.
   4. Socket classes. Serial port classes.
   5. Threads for TCP server and client, UDP, and serial that are used to communicate binary messages and strings.
   6. Command line executive classes for console and parameter and parameters files.
   7. A filtered print facility that can redirect to separate windows.
   8. Trace buffers that provide for instumentation of threads.
   9. Miscellaneous utility functions.

This library is written for Windows and Linux in Visual Studio and VS Code, using open folder cmake . The classes are based on either Win32 or Linux support for threads and sockets and such. The library is portable and the details of the provided functionality that are specific to Win32 or Linux are contained in a small set of `*_win.cpp` or `*_linux.cpp`files.

The solution contains the library project and a set of unit test and protorype projects.

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
risBaseDir.h                  -- base directory utilities
risBitUtils.h                 -- bit utilities
risByteBlob.h                 -- byte content binary blob
risByteBuffer.h               -- byte buffer for byte content binary message scheme
risByteContent.h              -- byte content base class for binary message scheme
risCmdLineCmd.h               -- command line command. is executed by a command line executive
risCmdLineConsole.h           -- execute command lines from the console, using a command line executive
risCmdLineExec.h              -- command line executive base class
risCmdLineFile.h              -- execute command lines from a file, using a command line executive
risCmdLineParms.h             -- support for command line parameters files
risCmdLineReader.h            -- support for reading command lines from stdin 
risCmdLineScript.h            -- execute command lines from a script file
risCmdLineString.h            -- execute command lines from a string, using a command line executive
risCmdLineTables.h            -- lookup tables that can be read from a command line file
risContainers.h               -- some legacy container class templates, queues, stacks ...
risContainers2.h              -- more of the same
risLCCriticalSection.h        -- locking critical sections
risLCPointerQueue.h           -- locking pointer queue
risLogic.h                    -- some and or not logic support for masks
risMemoryPtr.h                -- memory pointer support
risMonitor.h                  -- monitor variables
risNanoConvert.h              -- nanosecond time conversions
risNanoTime.h                 -- nanosecond time facility based on cpu clock
risNetInterfaces.h            -- network interface support
risNetPortDef.h               -- default network socket port definitions
risNetSessionStateList.h      -- session state list used by a tcp server to track tcp client connections
risNetSettings.h              -- settings used by the udp and tcp socket threads
risNetTcpMsgClientThread.h    -- tcp client thread based on byte content binary messages
risNetTcpMsgServerThread.h    -- tcp server thread based on byte content binary messages
risNetTcpMsgSocket.h          -- tcp socket for byte content binary messages 
risNetTcpServerHubSocket.h    -- tcp server hub socket, does accept/select
risNetUdpMsgSocket.h          -- udp socket for byte content binary messages 
risNetUdpMsgThread.h          -- udp peer thread based on byte content binary messages
risNetUdpStringSocket.h       -- udp thread that passes strings 
risNetUdpStringThread.h       -- udp peer thread based on strings
risPortableCalls.h            -- some system calls that are portable
risProgramTime.h              -- provides time from start of program based on hires cpu clock
risSequence.h                 -- state machines that generate sequences of ntuples of integers
risSerialHeaderBuffer.h       -- serial header buffer used to detect binary message headers
risSerialMsgPort.h            -- serial port for byte content binary messags
risSerialMsgThread.h          -- serial thread based on byte content binary messages
risSerialPort.h               -- serial port
risSerialSettings.h           -- serial port settings
risSerialStringPort.h         -- serial port for strings
risSerialStringThread.h       -- serial thread based on strings
risSharedMemory.h             -- shared memory support
risSleep.h                    -- sleep utilities
risSockets.h                  -- socket classes
risSRSWObjectQueue.h          -- single reader single writer object queue
risSRSWPointerQueue.h         -- single reader single pointer object queue
risSRSWStringQueue.h          -- single reader single writer string queue
risSRSWValueQueue.h           -- single reader single writer value queue
risStatistics.h               -- statistics classes for mean, stddev, min,max
risSystemCalls.h              -- system call support
risThreadsNotify.h            -- thread notifier
risThreadsPeriodicThread.h    -- periodic thread
risThreadsPriorities.h        -- thread priorities
risThreadsProcess.h           -- support for process priority and clocking
risThreadsQCall.h             -- qcalls are queue function calls, deferred procedure calls
risThreadsQCallThread.h       -- qcall thread, waits on a queue of calls and executes them serially
risThreadsRandomThread.h      -- thread that sleeps for a random time
risThreadsSynch.h             -- thread synchronization classes, mutex semaphores, event semaphores, etc
risThreadsThreads.h           -- thread base classes
risThreadsTimer.h             -- thread timer for periodic threads
risThreadsTimerThread.h       -- timer thread base class
risThreadsTwoThread.h         -- two thread base class, for threads that send a command and wait for a notification
risThreadsWaitable.h          -- waitable thread mechanism, can wait for a time or be aborted
risTimeMarker.h               -- time marker used to measure software timing
stdafx.h                      -- used for precompiled header files
risTrace.h                    -- trace buffer program interface
risTraceBuffer.h              -- trace buffer implementation
```
## Projects
``` markdown
ByteContent        -- unit test for byte content
Classes            -- constructor and destructor experiments
CmdLineTest        -- experiments for command line commands
CommonLib          -- common library used some of by the unit tests
eth0               -- network interface experiments
Experiment         -- experiments
FileFunctions      -- unit test for file function
Functions          -- function pointer experiments
LogFiles           --  unit test for log files
MyProgram1         -- program place holder
NamedSemaphore     -- unit test for named semaphores
Parms              -- unit test for parms file
PeriodicTest       --  unit test for periodic threads
PortResponder      -- prototype udp port responder thread
PrintTest          -- unit test for print facility
PrintView          -- PrintView.exe build
ProtoArgs          -- prototype for program arguments
ProtoSerial        -- prototype for byte content messages
ProtoSerialMsg     -- prototype for byte content message via serial port
ProtoSerialString  -- prototype for strings, serial port
ProtoTcpClientMsg  -- prototype for byte content message, tcp client
ProtoTcpServerMsg  -- prototype for byte content message, tcp client
ProtoUdpMsg        -- prototype for byte content message, udp
ProtoUdpString     -- prototype for strings, udp
QCallTest          -- unit test for qcall threads
RandomTest         -- unit test for random threads
Readline           -- unit test for readline
RisLib             -- realtime infrastructure library -- this is the library
Script             -- unit test for a script runner
Sequence           -- unit test for a sequence
SharedMemory       -- unit test for shared memory
Sleep              -- unit test for sleeps
Stash              -- old code stashed away
StringQueue        -- unit test string queue
TableTest          -- unit test for lookup tables
Test               -- general test project
ThreadNotify       -- unit test for thread notification class
ThreadQCall        -- unit test for qcall threads
ThreadSample       -- sample thread project
ThreadSynch        -- unit test for thread synchronization classes
ThreadTest         -- unit test for thread base classes
TimerTest          -- unit test for timer classes
TimerThreadTest    -- unit test for timer threads and priorites and cores
Trace              -- unit test for trace buffers
WaitableTest       -- unit test for waitables

```
 
## Build target platforms
``` markdown
This supports builds for the following platforms:

arm7    -- builds on windows for a debian arm7 target, tested on beaglebone black, raspberrypi4
arm8    -- builds on linux or wsl for a yocto aarch64 target, tested on veriscite mini dart board
ubuntu  -- builds on linux or wsl for a ubuntu target, tested on intel nuc and wsl
windows -- builds on windows for a windows target, tested on intel nuc
```
## CMakePresets
``` markdown
This uses CMakePresets.json, cmake 3.18 is required.

prime-windows  -- Target windows, install to c:/aaa_prime/RisLib
prime-x64      -- Target ubuntu, install to /opt/prime
prime-arm7     -- Target debian arm7 for beaglebone or rpi, install to /opt/prime
prime-arm8     -- Target varisite mini dart board, install to /opt/prime
```


## CMake Files
``` markdown
CMakeLists.txt               -- main cmake file
CMakePresets.json            -- cmake presets, requires cmake 3.18
imx8mm-toolchain.cmake       -- specific toolchain file
MyBeagleToolchain.cmake      -- specific toolchain file

MyCodeMacros.cmake           -- macros for compiler settings
MyInstall.cmake              -- include for cmake installs
MyLocalImports.cmake         -- macros for importing local libraries
MyOverrides.cmake            -- some cmake overrides
MyPostBuild.cmake            -- macros for post build copying
MyPrecompiledHeader.cmake    -- macros for using precompiled headers
MySpecial.cmake              -- special debuf stuff  
```

## Building from the command line 
``` markdown
Here's an example for cleaning, building, and installing. It uses the cmake preset prime-x64.

git clone git@github.com:chrishoen/Dev_RisLib.git
sudo chmod 777 Dev
mv Dev_RisLib rislib
cd rislib
git checkout develop

cmake --preset prime-x64
cmake --build out/build/prime-x64 --target clean
cmake --build out/build/prime-x64 -v -j 8
cmake --install out/build/prime-x64
```

## Note: Installing cmake 
``` markdown
To install cmake version 3.25 on ubuntu to /usr/bin, this worked for me.

wget https://github.com/Kitware/CMake/releases/download/v3.25.0-rc3/cmake-3.25.0-rc3-linux-x86_64.sh
sudo ./cmake-3.25.0-rc3-linux-x86_64.sh --skip-license --prefix=/usr


To install cmake version 3.19 for visual studio integration on ubuntu to /usr/bin, this worked for me.

wget https://github.com/microsoft/CMake/releases/download/v3.19.4268486/cmake-3.19.4268486-MSVC_2-Linux-x86.sh
sudo ./cmake-3.19.4268486-MSVC_2-Linux-x86.sh --skip-license --prefix=/usr
```
