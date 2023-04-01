cmake_minimum_required (VERSION 3.8)

project (ReadLine CXX)

my_find_src_files (src_files ReadLine)
add_executable (ReadLine ${src_files})
my_add_pch (ReadLine)

my_inc_import_RisLib(ReadLine)
my_lib_import_RisLib(ReadLine)


include("${MY_MACRO_DIR}/MyInstall.cmake")
