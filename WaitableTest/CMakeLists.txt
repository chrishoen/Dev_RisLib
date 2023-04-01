cmake_minimum_required (VERSION 3.8)

project (WaitableTest CXX)

my_find_src_files (src_files WaitableTest)
add_executable (WaitableTest ${src_files})
my_add_pch (WaitableTest)

my_inc_import_RisLib(WaitableTest)
my_inc_import_CommonLib(WaitableTest)


my_lib_import_CommonLib(WaitableTest)
my_lib_import_RisLib(WaitableTest)

include("${MY_MACRO_DIR}/MyInstall.cmake")
