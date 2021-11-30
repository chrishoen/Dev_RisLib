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

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS PeriodicTest DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(PeriodicTest)
endif()
