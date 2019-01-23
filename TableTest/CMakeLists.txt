cmake_minimum_required (VERSION 3.8)

project (TableTest CXX)

my_find_src_files (src_files TableTest)
add_executable (TableTest ${src_files})
my_add_compile_options (TableTest)
my_add_pch (TableTest)

target_include_directories(TableTest PRIVATE RisLib)
target_link_libraries(TableTest RisLib)
