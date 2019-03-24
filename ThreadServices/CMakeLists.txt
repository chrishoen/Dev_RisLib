cmake_minimum_required (VERSION 3.8)

project (ThreadServices CXX)

my_find_src_files (src_files ThreadServices)
add_executable (ThreadServices ${src_files})
my_add_compile_options (ThreadServices)
my_add_pch (ThreadServices)


my_inc_import_RisLib(ThreadServices)
my_inc_import_CommonLib(ThreadServices)

my_lib_import_CommonLib(ThreadServices)
my_lib_import_RisLib(ThreadServices)

