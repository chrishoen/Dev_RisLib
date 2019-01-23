cmake_minimum_required (VERSION 3.8)

project (ProtoUdp CXX)

my_find_src_files (src_files ProtoUdp)
add_executable (ProtoUdp ${src_files})
my_add_compile_options (ProtoUdp)
my_add_pch (ProtoUdp)

target_include_directories(ProtoUdp PRIVATE RisLib)
target_include_directories(ProtoUdp PRIVATE CommonLib)

target_link_libraries(ProtoUdp RisLib)
target_link_libraries(ProtoUdp CommonLib)
