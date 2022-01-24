cmake_minimum_required (VERSION 3.8)

project (SerialTest3 CXX)
my_find_src_files (src_files SerialTest3)
add_executable (SerialTest3 ${src_files})
my_add_compile_options (SerialTest3)
my_add_pch (SerialTest3)

my_inc_import_RisLib(SerialTest3)
my_inc_import_CommonLib(SerialTest3)

my_lib_import_CommonLib(SerialTest3)
my_lib_import_RisLib(SerialTest3)


if (MSVC)
   install(TARGETS SerialTest3 DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL ubuntu OR MYMODE STREQUAL wsl) 
   install(TARGETS SerialTest3 DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL beagle)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest3 pi@rpx:/opt/prime/bin)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest3 debian@bbx:/opt/prime/bin)")
elseif (MYMODE STREQUAL steno)
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/SerialTest3 root@stenotgt:/home/root/stenograph/bin)")
endif()

