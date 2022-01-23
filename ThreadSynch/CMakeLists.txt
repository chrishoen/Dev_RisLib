cmake_minimum_required (VERSION 3.8)

project (ThreadSynch CXX)

my_find_src_files (src_files ThreadSynch)
add_executable (ThreadSynch ${src_files})
my_add_compile_options (ThreadSynch)
my_add_pch (ThreadSynch)

my_inc_import_RisLib(ThreadSynch)

my_lib_import_RisLib(ThreadSynch)


if (MSVC)
   install(TARGETS ThreadSynch DESTINATION ${MyCopyBinToDir})
elseif (MYMODE STREQUAL ubuntu)
   my_post_build_copy_bin(ThreadSynch)
elseif (MYMODE STREQUAL beagle)
   my_post_build_scp_bin(ThreadSynch)
elseif (MYMODE STREQUAL steno)
   my_post_build_scp_bin(ThreadSynch)
endif()


