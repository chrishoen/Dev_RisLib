cmake_minimum_required (VERSION 3.8)

project (ProtoMsg CXX)

my_find_src_files (src_files ProtoMsg)
add_executable (ProtoMsg ${src_files})
my_add_compile_options (ProtoMsg)
my_add_pch (ProtoMsg)


my_inc_import_RisLib(ProtoMsg)
my_inc_import_CommonLib(ProtoMsg)

my_lib_import_CommonLib(ProtoMsg)
my_lib_import_RisLib(ProtoMsg)

if (MSVC)
   install(TARGETS ProtoMsg DESTINATION ${MyCopyBinToDir})
elseif (MyTargetType STREQUAL ubuntu)
   my_post_build_copy_bin(ProtoMsg)
elseif (MyTargetType STREQUAL beagle)
   my_post_build_scp_bin(ProtoMsg)
elseif (MyTargetType STREQUAL dart)
   my_post_build_scp_bin(ProtoMsg)
endif()
