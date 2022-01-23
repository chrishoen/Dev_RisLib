cmake_minimum_required (VERSION 3.8)

project (Test CXX)

my_find_src_files (src_files Test)
add_executable (Test ${src_files})
my_add_compile_options (Test)
my_add_pch (Test)

my_inc_import_RisLib(Test)
my_lib_import_RisLib(Test)




if (MSVC)
   install(TARGETS Test DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu OR MYMODE STREQUAL wsl) 
   install(TARGETS Test DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/Test pi@rpx:/opt/prime/bin)")
elseif (MYMODE STREQUAL dart)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/Test root@dart:/home/root/stenograph/bin)")
endif()

