#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

function(my_lib_import_Win32 _target)

   if (MSVC)
      target_link_libraries(${_target} setupapi)
   endif()

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
      target_link_libraries(${_target} RisLib)
      target_link_libraries(${_target} pthread)
      target_link_libraries(${_target} rt)
      target_link_libraries(${_target} readline)
   endif()

endfunction()

function(my_inc_import_RisLib _target)

   target_include_directories(${_target} PRIVATE $<TARGET_PROPERTY:RisLib,INTERFACE_INCLUDE_DIRECTORIES>)

endfunction()

#*******************************************************************************
#*******************************************************************************
#*******************************************************************************

