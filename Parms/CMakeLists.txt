cmake_minimum_required (VERSION 3.8)

project (Parms CXX)

my_find_src_files (src_files Parms)
add_executable (Parms ${src_files})
my_add_compile_options (Parms)
my_add_pch (Parms)

my_inc_import_RisLib(Parms)
my_lib_import_RisLib(Parms)

