cmake_minimum_required (VERSION 3.8)

project (TableTest CXX)

my_find_src_files (src_files TableTest)
add_executable (TableTest ${src_files})
my_add_compile_options (TableTest)
my_add_pch (TableTest)


my_inc_import_RisLib(TableTest)
my_lib_import_RisLib(TableTest)

