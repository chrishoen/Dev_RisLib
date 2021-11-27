#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_special1)

   message(STATUS "my_special***********************************************************BEGIN")
   if(MSVC)
      message(STATUS "my_special MSVC")
   elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
      message(STATUS "my_special LINUX")
      set (MyRsyncBin "rsync://main@devtgt:/prime/bin" PARENT_SCOPE)
      set (MyRsyncLib "rsync://main@devtgt:/prime/lib" PARENT_SCOPE)
   else()
      message(STATUS "my_special LINUX BEAGLE")
      set (MyRsyncBin "rsync://pi@devtgt:/prime/bin" PARENT_SCOPE)
      set (MyRsyncLib "rsync://pi@devtgt:/prime/lib" PARENT_SCOPE)
   endif()

   message(STATUS "my_special***********************************************************END")
endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

