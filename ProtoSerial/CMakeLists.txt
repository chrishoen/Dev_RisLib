cmake_minimum_required (VERSION 3.8)

project (ProtoSerial CXX)

my_find_src_files (src_files ProtoSerial)
add_executable (ProtoSerial ${src_files})
my_add_compile_options (ProtoSerial)
my_add_pch (ProtoSerial)


my_inc_import_RisLib(ProtoSerial)
my_inc_import_CommonLib(ProtoSerial)

my_lib_import_CommonLib(ProtoSerial)
my_lib_import_RisLib(ProtoSerial)



if (MSVC)
   install(TARGETS ProtoSerial DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu)
   install(TARGETS ProtoSerial DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/ProtoSerial pi@rpx:/opt/prime/bin)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/ProtoSerial debian@bbx:/opt/prime/bin)")
elseif (MYMODE STREQUAL dart)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/ProtoSerial root@dart:/home/root/stenograph/bin)")
endif()
