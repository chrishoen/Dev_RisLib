cmake_minimum_required (VERSION 3.8)

project (PrintView CXX)

my_find_src_files (src_files PrintView)
add_executable (PrintView ${src_files})
my_add_compile_options (PrintView)
my_add_pch (PrintView)

my_inc_import_RisLib(PrintView)
my_lib_import_RisLib(PrintView)

if(MSVC)
   install(TARGETS PrintView DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS PrintView DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(PrintView)
endif()
