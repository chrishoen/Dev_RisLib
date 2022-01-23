cmake_minimum_required (VERSION 3.8)

project (PrintTest CXX)

my_find_src_files (src_files PrintTest)
add_executable (PrintTest ${src_files})
my_add_compile_options (PrintTest)
my_add_pch (PrintTest)

my_inc_import_RisLib(PrintTest)
my_lib_import_RisLib(PrintTest)


if (MSVC)
   install(TARGETS PrintTest DESTINATION ${MyCopyBinToDir})
elseif (MYMODE STREQUAL ubuntu)
   my_post_build_copy_bin(PrintTest)
elseif (MYMODE STREQUAL beagle)
   my_post_build_scp_bin(PrintTest)
elseif (MYMODE STREQUAL steno)
endif()
