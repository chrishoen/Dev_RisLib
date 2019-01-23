cmake_minimum_required (VERSION 3.8)

project (ThreadSynch CXX)

my_find_src_files (src_files ThreadSynch)
add_executable (ThreadSynch ${src_files})
my_add_compile_options (ThreadSynch)
my_add_pch (ThreadSynch)

target_include_directories(ThreadSynch PRIVATE RisLib)
target_include_directories(ThreadSynch PRIVATE CommonLib)

target_link_libraries(ThreadSynch RisLib)
target_link_libraries(ThreadSynch CommonLib)
