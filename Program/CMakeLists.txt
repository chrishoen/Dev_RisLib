cmake_minimum_required (VERSION 3.8)

project (Program CXX)

my_find_src_files (src_files Program)
add_executable (Program ${src_files})
my_add_compile_options (Program)
my_add_pch (Program)

my_inc_import_RisLib(Program)
my_lib_import_RisLib(Program)

