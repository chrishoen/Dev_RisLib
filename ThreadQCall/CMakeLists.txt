cmake_minimum_required (VERSION 3.8)

project (ThreadQCall CXX)

my_find_src_files (src_files ThreadQCall)
add_executable (ThreadQCall ${src_files})
my_add_compile_options (ThreadQCall)
my_add_pch (ThreadQCall)

target_include_directories(ThreadQCall PRIVATE RisLib)
target_include_directories(ThreadQCall PRIVATE CommonLib)

target_link_libraries(ThreadQCall RisLib)
target_link_libraries(ThreadQCall CommonLib)
