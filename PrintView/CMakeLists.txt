cmake_minimum_required (VERSION 3.8)

project (PrintView CXX)

my_find_src_files (src_files PrintView)
add_executable (PrintView ${src_files})
my_add_compile_options (PrintView)
my_add_pch (PrintView)

my_inc_import_RisLib(PrintView)
my_lib_import_RisLib(PrintView)


if (MSVC)
   install(TARGETS PrintView DESTINATION ${MyCopyBinToDir})
elseif (MyTargetType STREQUAL ubuntu)
   my_post_build_copy_bin(PrintView)
elseif (MyTargetType STREQUAL beagle)
   my_post_build_scp_bin(PrintView)
elseif (MyTargetType STREQUAL dart)
endif()

