cmake_minimum_required (VERSION 3.8)

project (CmdLineTest1 CXX)

my_find_src_files (src_files CmdLineTest1)
add_executable (CmdLineTest1 ${src_files})
my_add_compile_options (CmdLineTest1)
my_add_pch (CmdLineTest1)

target_include_directories(CmdLineTest1 PRIVATE RisLib)
target_link_libraries(CmdLineTest1 RisLib)
