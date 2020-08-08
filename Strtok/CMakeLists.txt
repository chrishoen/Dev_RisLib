cmake_minimum_required (VERSION 3.8)

project (Strtok CXX)

my_find_src_files (src_files Strtok)
add_executable (Strtok ${src_files})
my_add_compile_options (Strtok)
my_add_pch (Strtok)


my_inc_import_RisLib(Strtok)
my_lib_import_RisLib(Strtok)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS Strtok DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  Strtok
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx Strtok rsync://debian@bbacm/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/Strtok)
endif()
