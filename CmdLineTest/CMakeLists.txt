cmake_minimum_required (VERSION 3.8)

project (CmdLineTest CXX)

my_find_src_files (src_files CmdLineTest)
add_executable (CmdLineTest ${src_files})
my_add_compile_options (CmdLineTest)
my_add_pch (CmdLineTest)

my_inc_import_RisLib(CmdLineTest)
my_lib_import_RisLib(CmdLineTest)
