cmake_minimum_required (VERSION 3.8)

project (ThreadSample CXX)

my_find_src_files (src_files ThreadSample)
add_executable (ThreadSample ${src_files})
my_add_compile_options (ThreadSample)
my_add_pch (ThreadSample)



my_inc_import_RisLib(ThreadSample)

my_lib_import_RisLib(ThreadSample)

