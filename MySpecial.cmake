#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_special1)

   message(STATUS "my_special1***********************************************************BEGIN")
   message(STATUS "CMAKE_SYSTEM                " ${CMAKE_SYSTEM})
   message(STATUS "CMAKE_SYSTEM_VERSION        " ${CMAKE_SYSTEM_VERSION})
   message(STATUS "CMAKE_BUILD_TYPE            " ${CMAKE_BUILD_TYPE})
   message(STATUS "CMAKE_BUILD_PARALLEL_LEVEL  " ${CMAKE_BUILD_PARALLEL_LEVEL})
   message(STATUS "DESTDIR                     " ${DESTDIR})
   message(STATUS "PROJECT_SOURCE_DIR          " ${PROJECT_SOURCE_DIR})

   message(STATUS "MYMODE                       " ${MYMODE})
   message(STATUS "MyTargetHostName            " ${MyTargetHostName})
   message(STATUS "MyCopyBinToDir              " ${MyCopyBinToDir})
   message(STATUS "MyCopyLibToDir              " ${MyCopyLibToDir})
   message(STATUS "MySudo                      " ${MySudo})
   message(STATUS "SRC_LINUX_DIR               " ${SRC_LINUX_DIR})

   message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
   message(STATUS "my_special1***********************************************************END")
endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

