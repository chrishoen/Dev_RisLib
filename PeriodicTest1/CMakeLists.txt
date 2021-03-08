cmake_minimum_required (VERSION 3.8)

project (PeriodicTest1 CXX)

my_find_src_files (src_files PeriodicTest1)
add_executable (PeriodicTest1 ${src_files})
my_add_compile_options (PeriodicTest1)
my_add_pch (PeriodicTest1)

my_inc_import_RisLib(PeriodicTest1)
my_inc_import_CommonLib(PeriodicTest1)

my_lib_import_CommonLib(PeriodicTest1)
my_lib_import_RisLib(PeriodicTest1)

if(MSVC)
   install(TARGETS PeriodicTest1 DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS PeriodicTest1 DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  PeriodicTest1
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx PeriodicTest1 rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/PeriodicTest1)
endif()
