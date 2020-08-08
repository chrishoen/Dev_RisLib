cmake_minimum_required (VERSION 3.8)

project (Test CXX)

my_find_src_files (src_files Test)
add_executable (Test ${src_files})
my_add_compile_options (Test)
my_add_pch (Test)


my_inc_import_RisLib(Test)
my_lib_import_RisLib(Test)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS Test DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  Test
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx Test rsync://debian@bbacm/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/Test)
endif()
