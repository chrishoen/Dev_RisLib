cmake_minimum_required (VERSION 3.8)

project (TimerTest CXX)

my_find_src_files (src_files TimerTest)
add_executable (TimerTest ${src_files})
my_add_compile_options (TimerTest)
my_add_pch (TimerTest)

my_inc_import_RisLib(TimerTest)
my_inc_import_CommonLib(TimerTest)

my_lib_import_CommonLib(TimerTest)
my_lib_import_RisLib(TimerTest)

include("${MY_MACRO_DIR}/MyInstall.cmake")
