cmake_minimum_required (VERSION 3.8)

project (ThreadTest CXX)

my_find_src_files (src_files ThreadTest)
add_executable (ThreadTest ${src_files})
my_add_compile_options (ThreadTest)
my_add_pch (ThreadTest)


my_inc_import_RisLib(ThreadTest)
my_inc_import_CommonLib(ThreadTest)

my_lib_import_CommonLib(ThreadTest)
my_lib_import_RisLib(ThreadTest)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS ThreadTest DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  ThreadTest
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx ThreadTest rsync://debian@bby/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/ThreadTest)
endif()
