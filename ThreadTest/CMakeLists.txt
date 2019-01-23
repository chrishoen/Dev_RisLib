cmake_minimum_required (VERSION 3.8)

project (ThreadTest CXX)

my_find_src_files (src_files ThreadTest)
add_executable (ThreadTest ${src_files})
my_add_compile_options (ThreadTest)
my_add_pch (ThreadTest)

target_include_directories(ThreadTest PRIVATE RisLib)
target_include_directories(ThreadTest PRIVATE CommonLib)

target_link_libraries(ThreadTest RisLib)
target_link_libraries(ThreadTest CommonLib)
