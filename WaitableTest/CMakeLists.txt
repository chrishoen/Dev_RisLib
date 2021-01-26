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

add_custom_command(
   TARGET WaitableTest
   POST_BUILD
   COMMAND rsync -azv --chmod=ugo=rwx WaitableTest rsync://debian@devtgt/prime/bin
   WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/WaitableTest)
