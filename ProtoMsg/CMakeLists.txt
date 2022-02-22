cmake_minimum_required (VERSION 3.8)

project (ProtoMsg CXX)

my_find_src_files (src_files ProtoMsg)
add_executable (ProtoMsg ${src_files})
my_add_compile_options (ProtoMsg)
my_add_pch (ProtoMsg)


my_inc_import_RisLib(ProtoMsg)
my_inc_import_CommonLib(ProtoMsg)

my_lib_import_CommonLib(ProtoMsg)
my_lib_import_RisLib(ProtoMsg)


if (MSVC)
   install(TARGETS ProtoMsg DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL "cpdev1" OR MYMODE STREQUAL "wsl") 
   install(TARGETS ProtoMsg DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL "beagle")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/ProtoMsg pi@rpx:/opt/prime/bin)")
elseif (MYMODE STREQUAL "steno-arm")


   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/ProtoMsg root@stenotgt:/home/root/stenograph/bin)")
endif()


