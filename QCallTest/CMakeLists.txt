cmake_minimum_required (VERSION 3.8)

project (QCallTest CXX)

my_find_src_files (src_files QCallTest)
add_executable (QCallTest ${src_files})
my_add_compile_options (QCallTest)
my_add_pch (QCallTest)


my_inc_import_RisLib(QCallTest)
my_lib_import_RisLib(QCallTest)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS QCallTest DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  QCallTest
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx QCallTest rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/QCallTest)
endif()
