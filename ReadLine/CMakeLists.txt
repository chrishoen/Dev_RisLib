cmake_minimum_required (VERSION 3.8)

project (ReadLine CXX)

my_find_src_files (src_files ReadLine)
add_executable (ReadLine ${src_files})
my_add_compile_options (ReadLine)
my_add_pch (ReadLine)

my_inc_import_RisLib(ReadLine)
my_lib_import_RisLib(ReadLine)

if (MSVC)
   install(TARGETS ReadLine DESTINATION ${MyCopyBinToDir})
elseif (MYMODE STREQUAL ubuntu)
   my_post_build_copy_bin(ReadLine)
elseif (MYMODE STREQUAL beagle)
   my_post_build_scp_bin(ReadLine)
elseif (MYMODE STREQUAL steno)
   my_post_build_scp_bin(ReadLine)
endif()

