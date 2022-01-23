cmake_minimum_required (VERSION 3.8)

project (Test CXX)

my_find_src_files (src_files Test)
add_executable (Test ${src_files})
my_add_compile_options (Test)
my_add_pch (Test)

my_inc_import_RisLib(Test)
my_lib_import_RisLib(Test)


set(SCP_CMD "scp ${CMAKE_CURRENT_BINARY_DIR}/Test pi@rpx:/opt/prime/bin")

message ("LINE101 ${SCP_CMD} ")

execute_process(COMMAND "${SCP_CMD}" COMMAND_ECHO STDOUT)
return()

install(CODE "execute_process(COMMAND ${SCP_CMD} )")



if (MSVC)
   install(TARGETS Test DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu OR MYMODE STREQUAL wsl) 
   install(TARGETS Test DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/Test pi@rpx:/opt/prime/bin)")
elseif (MYMODE STREQUAL steno)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/Test root@stenotgt:/home/root/stenograph/bin)")
endif()

return()

set(SCP_CMD scp ${CMAKE_CURRENT_BINARY_DIR}/Test pi@rpx:/opt/prime/bin)
message ("LINE101 ${SCP_CMD} ")

install(CODE "execute_process(COMMAND scp C:/Prime/DevelopLib/Dev_RisLib/out/build/beagle/Test/Test pi@rpx:/opt/prime/bin)")
return()

install(CODE "execute_process(COMMAND_ECHO STDOUT COMMAND ${SCP_CMD})")
return()



install(CODE "execute_process( COMMAND_ECHO STDOUT
   COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/Test pi@rpx:/opt/prime/bin)")
return()

install(CODE "execute_process( COMMAND_ECHO STDOUT
   COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/Test pi@rpx:/opt/prime/bin)")
return()

