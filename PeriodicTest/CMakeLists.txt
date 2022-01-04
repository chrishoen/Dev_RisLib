cmake_minimum_required (VERSION 3.8)

project (PeriodicTest CXX)

if(MSVC)
   return()
endif()

my_find_src_files (src_files PeriodicTest)
add_executable (PeriodicTest ${src_files})
my_add_compile_options (PeriodicTest)
my_add_pch (PeriodicTest)

my_inc_import_RisLib(PeriodicTest)
my_inc_import_CommonLib(PeriodicTest)

my_lib_import_CommonLib(PeriodicTest)
my_lib_import_RisLib(PeriodicTest)


if (MSVC)
elseif (MyTargetType STREQUAL ubuntu)
   my_post_build_copy_bin(PeriodicTest)
elseif (MyTargetType STREQUAL beagle)
   my_post_build_scp_bin(PeriodicTest)
elseif (MyTargetType STREQUAL dart)
   my_post_build_scp_bin(PeriodicTest)
endif()
