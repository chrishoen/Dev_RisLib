cmake_minimum_required (VERSION 3.8)

project (NamedSemaphore CXX)

my_find_src_files (src_files NamedSemaphore)
add_executable (NamedSemaphore ${src_files})
my_add_compile_options (NamedSemaphore)
my_add_pch (NamedSemaphore)


my_inc_import_RisLib(NamedSemaphore)
my_lib_import_RisLib(NamedSemaphore)


