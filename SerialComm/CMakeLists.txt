cmake_minimum_required (VERSION 3.8)

project (SerialComm CXX)

my_find_src_files (src_files SerialComm)
add_executable (SerialComm ${src_files})
my_add_compile_options (SerialComm)
my_add_pch (SerialComm)

target_include_directories(SerialComm PRIVATE RisLib)
target_include_directories(SerialComm PRIVATE CommonLib)

target_link_libraries(SerialComm RisLib)
target_link_libraries(SerialComm CommonLib)
