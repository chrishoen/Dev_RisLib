cmake_minimum_required (VERSION 3.8)

project (eth0 CXX)

my_find_src_files (src_files eth0)
add_executable (eth0 ${src_files})
my_add_compile_options (eth0)
my_add_pch (eth0)


my_inc_import_RisLib(eth0)
my_lib_import_RisLib(eth0)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS eth0 DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  eth0
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx eth0 rsync://debian@bby1/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/eth0)
endif()
