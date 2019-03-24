cmake_minimum_required (VERSION 3.8)

project (ProtoTcpClient CXX)

my_find_src_files (src_files ProtoTcpClient)
add_executable (ProtoTcpClient ${src_files})
my_add_compile_options (ProtoTcpClient)
my_add_pch (ProtoTcpClient)


my_inc_import_RisLib(ProtoTcpClient)
my_inc_import_CommonLib(ProtoTcpClient)

my_lib_import_CommonLib(ProtoTcpClient)
my_lib_import_RisLib(ProtoTcpClient)

