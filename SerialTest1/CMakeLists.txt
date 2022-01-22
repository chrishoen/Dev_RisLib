cmake_minimum_required (VERSION 3.8)

project (SerialTest1 CXX)
my_find_src_files (src_files SerialTest1)
add_executable (SerialTest1 ${src_files})
my_add_compile_options (SerialTest1)
my_add_pch (SerialTest1)

my_inc_import_RisLib(SerialTest1)
my_inc_import_CommonLib(SerialTest1)

my_lib_import_CommonLib(SerialTest1)
my_lib_import_RisLib(SerialTest1)



if (MSVC)
   install(TARGETS SerialTest1 DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu)
   install(TARGETS SerialTest1 DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest1 pi@rpx:/opt/prime/bin)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest1 debian@bbx:/opt/prime/bin)")
elseif (MYMODE STREQUAL dart)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest1 root@dart:/home/root/stenograph/bin)")
endif()
