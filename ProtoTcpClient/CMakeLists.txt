cmake_minimum_required (VERSION 3.8)

project (ProtoTcpClient CXX)

my_find_src_files (src_files ProtoTcpClient)
add_executable (ProtoTcpClient ${src_files})
my_add_compile_options (ProtoTcpClient)
my_add_pch (ProtoTcpClient)

target_include_directories(ProtoTcpClient PRIVATE RisLib)
target_include_directories(ProtoTcpClient PRIVATE CommonLib)

target_link_libraries(ProtoTcpClient RisLib)
target_link_libraries(ProtoTcpClient CommonLib)
