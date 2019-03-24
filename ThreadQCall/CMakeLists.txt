cmake_minimum_required (VERSION 3.8)

project (ThreadQCall CXX)

my_find_src_files (src_files ThreadQCall)
add_executable (ThreadQCall ${src_files})
my_add_compile_options (ThreadQCall)
my_add_pch (ThreadQCall)


my_inc_import_RisLib(ThreadQCall)
my_inc_import_CommonLib(ThreadQCall)

my_lib_import_CommonLib(ThreadQCall)
my_lib_import_RisLib(ThreadQCall)

