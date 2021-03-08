cmake_minimum_required (VERSION 3.8)

project (PeriodicTest2 CXX)

my_find_src_files (src_files PeriodicTest2)
add_executable (PeriodicTest2 ${src_files})
my_add_compile_options (PeriodicTest2)
my_add_pch (PeriodicTest2)

my_inc_import_RisLib(PeriodicTest2)
my_inc_import_CommonLib(PeriodicTest2)

my_lib_import_CommonLib(PeriodicTest2)
my_lib_import_RisLib(PeriodicTest2)

if(MSVC)
   install(TARGETS PeriodicTest2 DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS PeriodicTest2 DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  PeriodicTest2
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx PeriodicTest2 rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/PeriodicTest2)
endif()
