cmake_minimum_required (VERSION 3.8)

project (QCallTest CXX)

my_find_src_files (src_files QCallTest)
add_executable (QCallTest ${src_files})
my_add_compile_options (QCallTest)
my_add_pch (QCallTest)

my_inc_import_RisLib(QCallTest)
my_lib_import_RisLib(QCallTest)

if(MSVC)
   install(TARGETS QCallTest DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS QCallTest DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(QCallTest)
endif()
