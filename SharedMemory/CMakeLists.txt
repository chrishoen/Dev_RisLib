cmake_minimum_required (VERSION 3.8)

project (SharedMemory CXX)

my_find_src_files (src_files SharedMemory)
add_executable (SharedMemory ${src_files})
my_add_compile_options (SharedMemory)
my_add_pch (SharedMemory)

my_inc_import_RisLib(SharedMemory)
my_lib_import_RisLib(SharedMemory)


if (MSVC)
   install(TARGETS SharedMemory DESTINATION ${MyCopyBinToDir})
elseif (MyTargetType STREQUAL ubuntu)
   my_post_build_copy_bin(SharedMemory)
elseif (MyTargetType STREQUAL beagle)
   my_post_build_scp_bin(SharedMemory)
elseif (MyTargetType STREQUAL dart)
   my_post_build_scp_bin(SharedMemory)
endif()
