﻿cmake_minimum_required (VERSION 3.8)

project (Functions CXX)

my_find_src_files (src_files Functions)
add_executable (Functions ${src_files})
my_add_pch (Functions)

my_inc_import_RisLib(Functions)
my_lib_import_RisLib(Functions)


