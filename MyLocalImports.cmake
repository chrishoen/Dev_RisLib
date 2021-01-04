#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_CommonLib _target)

   target_link_libraries(${_target} CommonLib)

endfunction()

function(my_inc_import_CommonLib _target)

   target_include_directories(${_target} PRIVATE $<TARGET_PROPERTY:CommonLib,INTERFACE_INCLUDE_DIRECTORIES>)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_RisLib _target)

   if (MSVC)
      target_link_libraries(${_target} RisLib)
      target_link_libraries(${_target} ws2_32)
      target_link_libraries(${_target} winmm)
   else()
      if (CMAKE_SYSTEM_VERSION EQUAL 101)
         set (MyPThreadImportPath  "C:/Beagle/toolchain/arm-linux-gnueabihf/lib/libpthread.so")
         set (MyRTImportPath  "C:/Beagle/toolchain/arm-linux-gnueabihf/lib/librt.so")
      else()
         set (MyPThreadImportPath  "/usr/lib/x86_64-linux-gnu/libpthread.so")
         set (MyRTImportPath  "/usr/lib/x86_64-linux-gnu/librt.so")
      endif()
      add_library(PThreadLib SHARED IMPORTED)
      add_library(RTLib SHARED IMPORTED)
      set_target_properties(PThreadLib PROPERTIES IMPORTED_LOCATION ${MyPThreadImportPath})
      set_target_properties(RTLib PROPERTIES IMPORTED_LOCATION ${MyRTImportPath})

      target_link_libraries(${_target} RisLib)
      target_link_libraries(${_target} PThreadLib)
      target_link_libraries(${_target} RTLib)
   endif()
   return()

   if (MSVC)
      target_link_libraries(${_target} RisLib)
      target_link_libraries(${_target} ws2_32)
      target_link_libraries(${_target} winmm)
   else()
      target_link_libraries(${_target} RisLib)
   endif()
   return()

endfunction()

function(my_inc_import_RisLib _target)

   target_include_directories(${_target} PRIVATE $<TARGET_PROPERTY:RisLib,INTERFACE_INCLUDE_DIRECTORIES>)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_RisLib22 _target)

   target_link_libraries(${_target} RisLib)

   if (MSVC)
      target_link_libraries(${_target} ws2_32)
      target_link_libraries(${_target} winmm)
   else()
      set(THREADS_PREFER_PTHREAD_FLAG ON)
      find_package(Threads REQUIRED)
      target_link_libraries(${_target} Threads::Threads)
   endif()

endfunction()

