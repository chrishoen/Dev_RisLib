cmake_minimum_required (VERSION 3.8)

project (PeriodicTest CXX)

my_find_src_files (src_files PeriodicTest)
add_executable (PeriodicTest ${src_files})
my_add_compile_options (PeriodicTest)
my_add_pch (PeriodicTest)

my_inc_import_RisLib(PeriodicTest)
my_inc_import_CommonLib(PeriodicTest)

my_lib_import_CommonLib(PeriodicTest)
my_lib_import_RisLib(PeriodicTest)

if(MSVC)
   install(TARGETS PeriodicTest DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS PeriodicTest DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  PeriodicTest
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx PeriodicTest rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/PeriodicTest)
endif()
