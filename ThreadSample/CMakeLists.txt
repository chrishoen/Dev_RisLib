cmake_minimum_required (VERSION 3.8)

project (ThreadSample CXX)

my_find_src_files (src_files ThreadSample)
add_executable (ThreadSample ${src_files})
my_add_compile_options (ThreadSample)
my_add_pch (ThreadSample)

target_include_directories(ThreadSample PRIVATE RisLib)
target_include_directories(ThreadSample PRIVATE CommonLib)

target_link_libraries(ThreadSample RisLib)
target_link_libraries(ThreadSample CommonLib)
