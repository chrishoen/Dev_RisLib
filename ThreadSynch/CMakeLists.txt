cmake_minimum_required (VERSION 3.8)

project (ThreadSynch CXX)

my_find_src_files (src_files ThreadSynch)
add_executable (ThreadSynch ${src_files})
my_add_compile_options (ThreadSynch)
my_add_pch (ThreadSynch)

my_inc_import_RisLib(ThreadSynch)

my_lib_import_RisLib(ThreadSynch)

if(MSVC)
   install(TARGETS ThreadSynch DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS ThreadSynch DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(ThreadSynch)
endif()
