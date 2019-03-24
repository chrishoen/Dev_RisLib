cmake_minimum_required (VERSION 3.8)

project (Classes CXX)

my_find_src_files (src_files Classes)
add_executable (Classes ${src_files})
my_add_compile_options (Classes)
my_add_pch (Classes)

my_inc_import_RisLib(Classes)
my_lib_import_RisLib(Classes)

