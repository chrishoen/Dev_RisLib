cmake_minimum_required (VERSION 3.8)

project (ProtoMsg CXX)

my_find_src_files (src_files ProtoMsg)
add_executable (ProtoMsg ${src_files})
my_add_compile_options (ProtoMsg)
my_add_pch (ProtoMsg)


my_inc_import_RisLib(ProtoMsg)
my_inc_import_CommonLib(ProtoMsg)

my_lib_import_CommonLib(ProtoMsg)
my_lib_import_RisLib(ProtoMsg)

