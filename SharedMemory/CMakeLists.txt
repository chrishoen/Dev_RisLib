cmake_minimum_required (VERSION 3.8)

project (SharedMemory CXX)

my_find_src_files (src_files SharedMemory)
add_executable (SharedMemory ${src_files})
my_add_compile_options (SharedMemory)
my_add_pch (SharedMemory)

my_inc_import_RisLib(SharedMemory)
my_lib_import_RisLib(SharedMemory)


