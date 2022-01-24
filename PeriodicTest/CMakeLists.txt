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
   install(TARGETS PeriodicTest DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu OR MYMODE STREQUAL wsl) 
   install(TARGETS PeriodicTest DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/PeriodicTest pi@rpx:/opt/prime/bin)")
elseif (MYMODE STREQUAL steno)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/PeriodicTest root@stenotgt:/home/root/stenograph/bin)")
endif()


