cmake_minimum_required (VERSION 3.8)

project (Test CXX)

my_find_src_files (src_files Test)
add_executable (Test ${src_files})
my_add_compile_options (Test)
my_add_pch (Test)

my_inc_import_RisLib(Test)
my_lib_import_RisLib(Test)


if (MSVC)
   install(TARGETS Test DESTINATION ${MyCopyBinToDir})
elseif (MyTargetType STREQUAL ubuntu)
   my_post_build_copy_bin(Test)
elseif (MyTargetType STREQUAL beagle)
   my_post_build_scp_bin(Test)
elseif (MyTargetType STREQUAL dart)
   my_post_build_scp_bin(Test)
endif()

