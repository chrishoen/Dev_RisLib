cmake_minimum_required (VERSION 3.8)

project (SharedMemory CXX)

my_find_src_files (src_files SharedMemory)
add_executable (SharedMemory ${src_files})
my_add_compile_options (SharedMemory)
my_add_pch (SharedMemory)

my_inc_import_RisLib(SharedMemory)
my_lib_import_RisLib(SharedMemory)

if(MSVC)
   install(TARGETS SharedMemory DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS SharedMemory DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(SharedMemory)
endif()
