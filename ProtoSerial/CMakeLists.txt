cmake_minimum_required (VERSION 3.8)

project (ProtoSerial CXX)

my_find_src_files (src_files ProtoSerial)
add_executable (ProtoSerial ${src_files})
my_add_compile_options (ProtoSerial)
my_add_pch (ProtoSerial)

target_include_directories(ProtoSerial PRIVATE RisLib)
target_include_directories(ProtoSerial PRIVATE CommonLib)

target_link_libraries(ProtoSerial RisLib)
target_link_libraries(ProtoSerial CommonLib)
