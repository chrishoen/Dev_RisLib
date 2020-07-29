cmake_minimum_required (VERSION 3.8)

project (StringQueue CXX)

my_find_src_files (src_files StringQueue)
add_executable (StringQueue ${src_files})
my_add_compile_options (StringQueue)
my_add_pch (StringQueue)


my_inc_import_RisLib(StringQueue)
my_lib_import_RisLib(StringQueue)

