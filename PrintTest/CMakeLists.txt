cmake_minimum_required (VERSION 3.8)

project (PrintTest CXX)

my_find_src_files (src_files PrintTest)
add_executable (PrintTest ${src_files})
my_add_compile_options (PrintTest)
my_add_pch (PrintTest)

my_inc_import_RisLib(PrintTest)
my_lib_import_RisLib(PrintTest)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS PrintTest DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  PrintTest
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx PrintTest rsync://debian@bbacm/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/PrintTest)
endif()
