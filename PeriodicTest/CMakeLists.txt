cmake_minimum_required (VERSION 3.8)

project (PeriodicTest CXX)

if(MSVC)
   return()
endif()

my_find_src_files (src_files PeriodicTest)
add_executable (PeriodicTest ${src_files})
my_add_compile_options (PeriodicTest)
my_add_pch (PeriodicTest)

my_inc_import_RisLib(PeriodicTest)
my_inc_import_CommonLib(PeriodicTest)

my_lib_import_CommonLib(PeriodicTest)
my_lib_import_RisLib(PeriodicTest)


if (MSVC)
elseif (MyTargetType STREQUAL ubuntu)
   install(TARGETS PeriodicTest DESTINATION "/opt/prime/bin")
elseif (MyTargetType STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/PeriodicTest pi@rpx:/opt/prime/bin)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/PeriodicTest debian@bbx:/opt/prime/bin)")
elseif (MyTargetType STREQUAL dart)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/PeriodicTest root@dart:/home/root/stenograph/bin)")
endif()
