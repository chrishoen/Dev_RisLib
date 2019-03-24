cmake_minimum_required (VERSION 3.8)

project (ThreadTest CXX)

my_find_src_files (src_files ThreadTest)
add_executable (ThreadTest ${src_files})
my_add_compile_options (ThreadTest)
my_add_pch (ThreadTest)


my_inc_import_RisLib(ThreadTest)
my_inc_import_CommonLib(ThreadTest)

my_lib_import_CommonLib(ThreadTest)
my_lib_import_RisLib(ThreadTest)

