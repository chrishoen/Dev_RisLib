cmake_minimum_required (VERSION 3.8)

project (SerialTest2 CXX)
my_find_src_files (src_files SerialTest2)
add_executable (SerialTest2 ${src_files})
my_add_compile_options (SerialTest2)
my_add_pch (SerialTest2)

my_inc_import_RisLib(SerialTest2)
my_inc_import_CommonLib(SerialTest2)

my_lib_import_CommonLib(SerialTest2)
my_lib_import_RisLib(SerialTest2)



if (MSVC)
   install(TARGETS SerialTest2 DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu)
   install(TARGETS SerialTest2 DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest2 pi@rpx:/opt/prime/bin)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest2 debian@bbx:/opt/prime/bin)")
elseif (MYMODE STREQUAL dart)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest2 root@dart:/home/root/stenograph/bin)")
endif()
