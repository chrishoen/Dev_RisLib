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

function(my_inc_import_RisLib _target)

   target_include_directories(${_target} PRIVATE $<TARGET_PROPERTY:RisLib,INTERFACE_INCLUDE_DIRECTORIES>)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

