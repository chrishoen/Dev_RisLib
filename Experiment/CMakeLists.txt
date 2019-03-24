cmake_minimum_required (VERSION 3.8)

project (Experiment CXX)

my_find_src_files (src_files Experiment)
add_executable (Experiment ${src_files})
my_add_compile_options (Experiment)
my_add_pch (Experiment)

my_inc_import_RisLib(Experiment)
my_lib_import_RisLib(Experiment)

