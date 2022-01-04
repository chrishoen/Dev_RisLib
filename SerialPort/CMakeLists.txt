cmake_minimum_required (VERSION 3.8)

project (SerialPort CXX)

my_find_src_files (src_files SerialPort)
add_executable (SerialPort ${src_files})
my_add_compile_options (SerialPort)
my_add_pch (SerialPort)

my_inc_import_RisLib(SerialPort)
my_inc_import_CommonLib(SerialPort)

my_lib_import_CommonLib(SerialPort)
my_lib_import_RisLib(SerialPort)


if (MSVC)
   install(TARGETS SerialPort DESTINATION ${MyCopyBinToDir})
elseif (MyTargetType STREQUAL ubuntu)
   my_post_build_copy_bin(SerialPort)
elseif (MyTargetType STREQUAL beagle)
   my_post_build_scp_bin(SerialPort)
elseif (MyTargetType STREQUAL dart)
   my_post_build_scp_bin(SerialPort)
endif()

