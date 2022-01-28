cmake_minimum_required (VERSION 3.8)

project (PrintTest CXX)

my_find_src_files (src_files PrintTest)
add_executable (PrintTest ${src_files})
my_add_compile_options (PrintTest)
my_add_pch (PrintTest)

my_inc_import_RisLib(PrintTest)
my_lib_import_RisLib(PrintTest)


if (MSVC)
   install(TARGETS PrintTest DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu OR MYMODE STREQUAL wsl) 
   install(TARGETS PrintTest DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/PrintTest pi@rpx:/opt/prime/bin)")
elseif (MYMODE STREQUAL steno)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/PrintTest root@stenotgt:/home/root/stenograph/bin)")
endif()
