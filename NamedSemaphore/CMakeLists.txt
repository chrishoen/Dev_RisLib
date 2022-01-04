cmake_minimum_required (VERSION 3.8)

project (NamedSemaphore CXX)

my_find_src_files (src_files NamedSemaphore)
add_executable (NamedSemaphore ${src_files})
my_add_compile_options (NamedSemaphore)
my_add_pch (NamedSemaphore)


my_inc_import_RisLib(NamedSemaphore)
my_lib_import_RisLib(NamedSemaphore)


if (MSVC)
   install(TARGETS NamedSemaphore DESTINATION ${MyCopyBinToDir})
elseif (MyTargetType STREQUAL ubuntu)
   my_post_build_copy_bin(NamedSemaphore)
elseif (MyTargetType STREQUAL beagle)
   my_post_build_scp_bin(NamedSemaphore)
elseif (MyTargetType STREQUAL dart)
   my_post_build_scp_bin(NamedSemaphore)
endif()

