cmake_minimum_required (VERSION 3.8)

project (ThreadServices CXX)

my_find_src_files (src_files ThreadServices)
add_executable (ThreadServices ${src_files})
my_add_compile_options (ThreadServices)
my_add_pch (ThreadServices)

target_include_directories(ThreadServices PRIVATE RisLib)
target_include_directories(ThreadServices PRIVATE CommonLib)

target_link_libraries(ThreadServices RisLib)
target_link_libraries(ThreadServices CommonLib)
