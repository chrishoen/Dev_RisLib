﻿cmake_minimum_required (VERSION 3.8)

include(MyCMake.cmake)

project ("Dev_RisLib")

add_subdirectory ("RisLib")
add_subdirectory ("Test")
return()

add_subdirectory ("ByteContent")
add_subdirectory ("Classes")
add_subdirectory ("CmdLineTest1")
add_subdirectory ("CommonLib")
add_subdirectory ("Experiment")
add_subdirectory ("Functions")
add_subdirectory ("Parms")
add_subdirectory ("PrintTest")
add_subdirectory ("PrintView")
add_subdirectory ("Program")
add_subdirectory ("ProtoMsg")
add_subdirectory ("ProtoSerial")
add_subdirectory ("ProtoTcpClient")
add_subdirectory ("ProtoTcpServer")
add_subdirectory ("ProtoUdp")
add_subdirectory ("RisLib")
add_subdirectory ("SerialComm")
add_subdirectory ("TableTest")
add_subdirectory ("Test")
add_subdirectory ("ThreadNotify")
add_subdirectory ("ThreadQCall")
add_subdirectory ("ThreadSample")
add_subdirectory ("ThreadServices")
add_subdirectory ("ThreadSynch")
add_subdirectory ("ThreadTest")
add_subdirectory ("TimerTest")
add_subdirectory ("TimerThreadTest")