cmake_minimum_required (VERSION 3.8)

project (ProtoString CXX)

my_find_src_files (src_files ProtoString)
add_executable (ProtoString ${src_files})
my_add_compile_options (ProtoString)
my_add_pch (ProtoString)


my_inc_import_RisLib(ProtoString)
my_inc_import_CommonLib(ProtoString)

my_lib_import_CommonLib(ProtoString)
my_lib_import_RisLib(ProtoString)

