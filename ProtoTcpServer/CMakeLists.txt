cmake_minimum_required (VERSION 3.8)

project (ProtoTcpServer CXX)

my_find_src_files (src_files ProtoTcpServer)
add_executable (ProtoTcpServer ${src_files})
my_add_compile_options (ProtoTcpServer)
my_add_pch (ProtoTcpServer)

target_include_directories(ProtoTcpServer PRIVATE RisLib)
target_include_directories(ProtoTcpServer PRIVATE CommonLib)

target_link_libraries(ProtoTcpServer RisLib)
target_link_libraries(ProtoTcpServer CommonLib)
