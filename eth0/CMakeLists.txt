cmake_minimum_required (VERSION 3.8)

project (eth0 CXX)

if(MSVC)
   return()
endif()

my_find_src_files (src_files eth0)
add_executable (eth0 ${src_files})
my_add_pch (eth0)

my_inc_import_RisLib(eth0)
my_lib_import_RisLib(eth0)




