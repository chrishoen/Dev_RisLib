cmake_minimum_required (VERSION 3.8)

project (WaitableTest CXX)

my_find_src_files (src_files WaitableTest)
add_executable (WaitableTest ${src_files})
my_add_compile_options (WaitableTest)
my_add_pch (WaitableTest)


my_inc_import_RisLib(WaitableTest)
my_inc_import_CommonLib(WaitableTest)

my_lib_import_CommonLib(WaitableTest)
my_lib_import_RisLib(WaitableTest)


if(MSVC)
   install(TARGETS WaitableTest DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS WaitableTest DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(WaitableTest)
endif()
