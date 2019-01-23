cmake_minimum_required (VERSION 3.8)

project (Parms CXX)

my_find_src_files (src_files Parms)
add_executable (Parms ${src_files})
my_add_compile_options (Parms)
my_add_pch (Parms)

target_include_directories(Parms PRIVATE RisLib)
target_link_libraries(Parms RisLib)
