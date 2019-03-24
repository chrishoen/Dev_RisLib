cmake_minimum_required (VERSION 3.8)

project (ProtoUdp CXX)

my_find_src_files (src_files ProtoUdp)
add_executable (ProtoUdp ${src_files})
my_add_compile_options (ProtoUdp)
my_add_pch (ProtoUdp)


my_inc_import_RisLib(ProtoUdp)
my_inc_import_CommonLib(ProtoUdp)

my_lib_import_CommonLib(ProtoUdp)
my_lib_import_RisLib(ProtoUdp)

