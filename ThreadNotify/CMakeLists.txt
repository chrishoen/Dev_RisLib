cmake_minimum_required (VERSION 3.8)

project (ThreadNotify CXX)

my_find_src_files (src_files ThreadNotify)
add_executable (ThreadNotify ${src_files})
my_add_compile_options (ThreadNotify)
my_add_pch (ThreadNotify)

target_include_directories(ThreadNotify PRIVATE RisLib)
target_include_directories(ThreadNotify PRIVATE CommonLib)

target_link_libraries(ThreadNotify RisLib)
target_link_libraries(ThreadNotify CommonLib)
