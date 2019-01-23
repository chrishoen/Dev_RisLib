cmake_minimum_required (VERSION 3.8)

project (Classes CXX)

my_find_src_files (src_files Classes)
add_executable (Classes ${src_files})
my_add_compile_options (Classes)
my_add_pch (Classes)

target_include_directories(Classes PRIVATE RisLib)
target_link_libraries(Classes RisLib)
