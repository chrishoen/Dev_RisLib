cmake_minimum_required (VERSION 3.8)

project (ThreadNotify CXX)

my_find_src_files (src_files ThreadNotify)
add_executable (ThreadNotify ${src_files})
my_add_compile_options (ThreadNotify)
my_add_pch (ThreadNotify)


my_inc_import_RisLib(ThreadNotify)
my_inc_import_CommonLib(ThreadNotify)

my_lib_import_CommonLib(ThreadNotify)
my_lib_import_RisLib(ThreadNotify)

