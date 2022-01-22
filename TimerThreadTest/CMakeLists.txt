cmake_minimum_required (VERSION 3.8)

project (TimerThreadTest CXX)

my_find_src_files (src_files TimerThreadTest)
add_executable (TimerThreadTest ${src_files})
my_add_compile_options (TimerThreadTest)
my_add_pch (TimerThreadTest)

my_inc_import_RisLib(TimerThreadTest)
my_inc_import_CommonLib(TimerThreadTest)

my_lib_import_CommonLib(TimerThreadTest)
my_lib_import_RisLib(TimerThreadTest)


if (MSVC)
   install(TARGETS TimerThreadTest DESTINATION ${MyCopyBinToDir})
elseif (MYMODE STREQUAL ubuntu)
   my_post_build_copy_bin(TimerThreadTest)
elseif (MYMODE STREQUAL beagle)
   my_post_build_scp_bin(TimerThreadTest)
elseif (MYMODE STREQUAL dart)
   my_post_build_scp_bin(TimerThreadTest)
endif()


