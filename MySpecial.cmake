#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_special1)

   message(STATUS "my_special1***********************************************************BEGIN")
   message(STATUS "MYMODE                 " ${MYMODE})
   message(STATUS "CMAKE_SYSTEM           " ${CMAKE_SYSTEM})
   message(STATUS "CMAKE_BUILD_TYPE       " ${CMAKE_BUILD_TYPE})
   message(STATUS "CMAKE_INSTALL_PREFIX   " ${CMAKE_INSTALL_PREFIX})
   message(STATUS "PROJECT_SOURCE_DIR     " ${PROJECT_SOURCE_DIR})

   message(STATUS "Using toolchain file:  " ${CMAKE_TOOLCHAIN_FILE})
   message(STATUS "my_special1***********************************************************END")
endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

