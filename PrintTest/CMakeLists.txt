cmake_minimum_required (VERSION 3.8)

project (PrintTest CXX)

my_find_src_files (src_files PrintTest)
add_executable (PrintTest ${src_files})
my_add_compile_options (PrintTest)
my_add_pch (PrintTest)

my_inc_import_RisLib(PrintTest)
my_lib_import_RisLib(PrintTest)

