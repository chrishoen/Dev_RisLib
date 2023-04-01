cmake_minimum_required (VERSION 3.8)

project (QCallTest CXX)

my_find_src_files (src_files QCallTest)
add_executable (QCallTest ${src_files})
my_add_pch (QCallTest)

my_inc_import_RisLib(QCallTest)
my_lib_import_RisLib(QCallTest)


include("${MY_MACRO_DIR}/MyInstall.cmake")
