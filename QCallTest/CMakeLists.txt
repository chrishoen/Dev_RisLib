cmake_minimum_required (VERSION 3.8)

project (QCallTest CXX)

my_find_src_files (src_files QCallTest)
add_executable (QCallTest ${src_files})
my_add_compile_options (QCallTest)
my_add_pch (QCallTest)

my_inc_import_RisLib(QCallTest)
my_lib_import_RisLib(QCallTest)

if (MSVC)
   install(TARGETS QCallTest DESTINATION ${MyCopyBinToDir})
elseif (MYMODE STREQUAL ubuntu)
   my_post_build_copy_bin(QCallTest)
elseif (MYMODE STREQUAL beagle)
   my_post_build_scp_bin(QCallTest)
elseif (MYMODE STREQUAL dart)
   my_post_build_scp_bin(QCallTest)
endif()
