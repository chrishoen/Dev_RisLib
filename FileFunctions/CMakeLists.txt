cmake_minimum_required (VERSION 3.8)

project (FileFunctions CXX)

my_find_src_files (src_files FileFunctions)
add_executable (FileFunctions ${src_files})
my_add_compile_options (FileFunctions)
my_add_pch (FileFunctions)


my_inc_import_RisLib(FileFunctions)
my_lib_import_RisLib(FileFunctions)


if (MSVC)
   install(TARGETS FileFunctions DESTINATION ${MyCopyBinToDir})
elseif (MYMODE STREQUAL ubuntu)
   my_post_build_copy_bin(FileFunctions)
elseif (MYMODE STREQUAL beagle)
   my_post_build_scp_bin(FileFunctions)
elseif (MYMODE STREQUAL steno)
   my_post_build_scp_bin(FileFunctions)
endif()

