cmake_minimum_required (VERSION 3.8)

project (SerialTest4 CXX)
my_find_src_files (src_files SerialTest4)
add_executable (SerialTest4 ${src_files})
my_add_compile_options (SerialTest4)
my_add_pch (SerialTest4)

my_inc_import_RisLib(SerialTest4)
my_inc_import_CommonLib(SerialTest4)

my_lib_import_CommonLib(SerialTest4)
my_lib_import_RisLib(SerialTest4)



if (MSVC)
   install(TARGETS SerialTest4 DESTINATION "C:/aaa_prime/RisLib")
elseif (MyTargetType STREQUAL ubuntu)
   install(TARGETS SerialTest4 DESTINATION "/opt/prime/bin")
elseif (MyTargetType STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest4 pi@rpx:/opt/prime/bin)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest4 debian@bbx:/opt/prime/bin)")
elseif (MyTargetType STREQUAL dart)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest4 root@dart:/home/root/stenograph/bin)")
endif()
