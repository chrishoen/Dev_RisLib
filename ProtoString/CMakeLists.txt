cmake_minimum_required (VERSION 3.8)

project (ProtoString CXX)

my_find_src_files (src_files ProtoString)
add_executable (ProtoString ${src_files})
my_add_compile_options (ProtoString)
my_add_pch (ProtoString)

target_include_directories(ProtoString PRIVATE RisLib)
target_include_directories(ProtoString PRIVATE CommonLib)

target_link_libraries(ProtoString RisLib)
target_link_libraries(ProtoString CommonLib)
