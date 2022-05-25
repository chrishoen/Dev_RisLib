cmake_minimum_required (VERSION 3.8)

set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/MyOverrides.cmake")
include(MyCodeMacros.cmake)
include(MyLocalImports.cmake)
include(MyPrecompiledHeader.cmake)
include(MySpecial.cmake)

project ("Dev_RisLib")
my_special1() 


add_subdirectory ("RisLib")
add_subdirectory ("CommonLib")
add_subdirectory ("ProtoUdpMsg")
return()

add_subdirectory ("RisLib")
return()

add_subdirectory ("RisLib")
add_subdirectory ("Script")
return()

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
add_subdirectory ("SerialTest3")
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

add_subdirectory ("MyProgram1")
return()

add_subdirectory ("RisLib")
return()

add_subdirectory ("Test")
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
add_subdirectory ("Program")
add_subdirectory ("ProtoMsg")
add_subdirectory ("ProtoSerial")
add_subdirectory ("ProtoTcpClient")
add_subdirectory ("ProtoTcpServer")
add_subdirectory ("ProtoUdp")
add_subdirectory ("QCallTest")
add_subdirectory ("RisLib")
add_subdirectory ("SerialComm")
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






