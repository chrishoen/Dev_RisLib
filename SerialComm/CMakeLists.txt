cmake_minimum_required (VERSION 3.8)

project (SerialComm CXX)

my_find_src_files (src_files SerialComm)
add_executable (SerialComm ${src_files})
my_add_compile_options (SerialComm)
my_add_pch (SerialComm)


my_inc_import_RisLib(SerialComm)
my_inc_import_CommonLib(SerialComm)

my_lib_import_CommonLib(SerialComm)
my_lib_import_RisLib(SerialComm)

