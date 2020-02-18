cmake_minimum_required (VERSION 3.8)

project (FileFunctions CXX)

my_find_src_files (src_files FileFunctions)
add_executable (FileFunctions ${src_files})
my_add_compile_options (FileFunctions)
my_add_pch (FileFunctions)


my_inc_import_RisLib(FileFunctions)
my_lib_import_RisLib(FileFunctions)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS FileFunctions DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  FileFunctions
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx FileFunctions rsync://debian@bby1/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/FileFunctions)
endif()
