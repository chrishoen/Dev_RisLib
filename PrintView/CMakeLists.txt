cmake_minimum_required (VERSION 3.8)

project (PrintView CXX)

my_find_src_files (src_files PrintView)
add_executable (PrintView ${src_files})
my_add_compile_options (PrintView)
my_add_pch (PrintView)

target_include_directories(PrintView PRIVATE RisLib)
target_link_libraries(PrintView RisLib)
