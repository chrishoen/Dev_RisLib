cmake_minimum_required (VERSION 3.8)

project (Functions CXX)

my_find_src_files (src_files Functions)
add_executable (Functions ${src_files})
my_add_compile_options (Functions)
my_add_pch (Functions)

target_include_directories(Functions PRIVATE RisLib)
target_link_libraries(Functions RisLib)
