﻿cmake_minimum_required (VERSION 3.8)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(MY_MACRO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zzzMacros")
set(CMAKE_USER_MAKE_RULES_OVERRIDE "${MY_MACRO_DIR}/MyOverrides.cmake")
include("${MY_MACRO_DIR}/MyCodeMacros.cmake")
include("${MY_MACRO_DIR}/MyLocalImports.cmake")
include("${MY_MACRO_DIR}/MyPrecompiledHeader.cmake")
include("${MY_MACRO_DIR}/MySpecial.cmake")

project ("Dev_RisLib")
my_append_compile_flags()
my_print_cmake_vars()

if (MYSTANDALONE)
  add_subdirectory ("RisLib")
  return()
endif()



add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpMsg")
add_subdirectory ("ProtoUdpString")
add_subdirectory ("ProtoUdpData")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("TimerThreadTest")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("Test")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpMsg")
add_subdirectory ("ProtoUdpString")
add_subdirectory ("ProtoUdpData")
return()

add_subdirectory ("RisLib")
return()


add_subdirectory ("RisLib")
add_subdirectory ("Sleep")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ThreadNotify")
return()
add_subdirectory ("RisLib")
add_subdirectory ("Trace")
return()

add_subdirectory ("ExampleQCall")
add_subdirectory ("RisLib")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("SppClientMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoSerialMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoSerialString")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
add_subdirectory ("ProtoUdpMsg")
add_subdirectory ("ProtoUdpString")
return()

add_subdirectory ("RisLib")
add_subdirectory ("Test")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpData")
return()


add_subdirectory ("RisLib")
add_subdirectory ("FileFunctions")
return()


add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ByteContent")
add_subdirectory ("Classes")
add_subdirectory ("CmdLineTest")
add_subdirectory ("eth0")
add_subdirectory ("Experiment")
add_subdirectory ("FileFunctions")
add_subdirectory ("Functions")
add_subdirectory ("NamedSemaphore")
add_subdirectory ("Parms")
add_subdirectory ("PeriodicTest")
add_subdirectory ("PrintTest")
add_subdirectory ("PrintView")
add_subdirectory ("ProtoMsg")
add_subdirectory ("ProtoSerialMsg")
add_subdirectory ("ProtoSerialString")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
add_subdirectory ("ProtoUdpMsg")
add_subdirectory ("ProtoUdpString")
add_subdirectory ("QCallTest")
add_subdirectory ("RandomTest")
add_subdirectory ("SharedMemory")
add_subdirectory ("StringQueue")
add_subdirectory ("TableTest")
add_subdirectory ("Test")
add_subdirectory ("ThreadNotify")
add_subdirectory ("ThreadQCall")
add_subdirectory ("ThreadSample")
add_subdirectory ("ThreadSynch")
add_subdirectory ("ThreadTest")
add_subdirectory ("TimerTest")
add_subdirectory ("TimerThreadTest")
add_subdirectory ("WaitableTest")
return()

add_subdirectory ("RisLib")
return()

add_subdirectory ("RisLib")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("Test")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("Script")
return()

add_subdirectory ("ByteContent")
add_subdirectory ("Classes")
add_subdirectory ("CmdLineTest")
add_subdirectory ("CommonLib")
add_subdirectory ("eth0")
add_subdirectory ("Experiment")
add_subdirectory ("FileFunctions")
add_subdirectory ("Functions")
add_subdirectory ("NamedSemaphore")
add_subdirectory ("Parms")
add_subdirectory ("PeriodicTest")
add_subdirectory ("PrintTest")
add_subdirectory ("PrintView")
add_subdirectory ("ProtoMsg")
add_subdirectory ("ProtoSerialMsg")
add_subdirectory ("ProtoSerialString")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
add_subdirectory ("ProtoUdpMsg")
add_subdirectory ("ProtoUdpString")
add_subdirectory ("QCallTest")
add_subdirectory ("RandomTest")
add_subdirectory ("RisLib")
add_subdirectory ("SharedMemory")
add_subdirectory ("StringQueue")
add_subdirectory ("TableTest")
add_subdirectory ("Test")
add_subdirectory ("ThreadNotify")
add_subdirectory ("ThreadQCall")
add_subdirectory ("ThreadSample")
add_subdirectory ("ThreadSynch")
add_subdirectory ("ThreadTest")
add_subdirectory ("TimerTest")
add_subdirectory ("TimerThreadTest")
add_subdirectory ("WaitableTest")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpString")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoSerialMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("PortResponder")
return()

add_subdirectory ("MyProgram1")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoTcpClientMsg")
add_subdirectory ("ProtoTcpServerMsg")
add_subdirectory ("ProtoUdpMsg")
return()

add_subdirectory ("RisLib")
add_subdirectory ("ProtoSerialMsg")
add_subdirectory ("RisLib")
add_subdirectory ("LogFiles")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("PeriodicTest")
add_subdirectory ("RandomTest")
return()

add_subdirectory ("RisLib")
add_subdirectory ("Trace")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("PeriodicTest")
add_subdirectory ("RandomTest")
add_subdirectory ("Test")
add_subdirectory ("ThreadTest")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("PeriodicTest")
add_subdirectory ("Test")
return()

add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoSerial")
return()

add_subdirectory ("Test")
return()

add_subdirectory ("RisLib")
return()

add_subdirectory ("CommonLib")
add_subdirectory ("RisLib")
add_subdirectory ("PeriodicTest")
return()

add_subdirectory ("CommonLib")
add_subdirectory ("RisLib")
add_subdirectory ("SerialPort")
return()

add_subdirectory ("CommonLib")
add_subdirectory ("RisLib")
add_subdirectory ("PeriodicTest")
return()


add_subdirectory ("CommonLib")
add_subdirectory ("RisLib")
add_subdirectory ("PeriodicTest")
add_subdirectory ("WaitableTest")
return()

add_subdirectory ("RisLib")
return()






