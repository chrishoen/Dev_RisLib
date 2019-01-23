cmake_minimum_required (VERSION 3.8)

project (ProtoMsg CXX)

my_find_src_files (src_files ProtoMsg)
add_executable (ProtoMsg ${src_files})
my_add_compile_options (ProtoMsg)
my_add_pch (ProtoMsg)

target_include_directories(ProtoMsg PRIVATE RisLib)
target_include_directories(ProtoMsg PRIVATE CommonLib)

target_link_libraries(ProtoMsg RisLib)
target_link_libraries(ProtoMsg CommonLib)
