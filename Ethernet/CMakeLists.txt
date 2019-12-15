cmake_minimum_required (VERSION 3.8)

project (Ethernet CXX)

my_find_src_files (src_files Ethernet)
add_executable (Ethernet ${src_files})
my_add_compile_options (Ethernet)
my_add_pch (Ethernet)


my_inc_import_RisLib(Ethernet)
my_lib_import_RisLib(Ethernet)

