cmake_minimum_required (VERSION 3.8)

project (PrintTest CXX)

my_find_src_files (src_files PrintTest)
add_executable (PrintTest ${src_files})
my_add_compile_options (PrintTest)
my_add_pch (PrintTest)

my_inc_import_RisLib(PrintTest)
my_lib_import_RisLib(PrintTest)

if(MSVC)
   install(TARGETS PrintTest DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS PrintTest DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(PrintTest)
endif()
