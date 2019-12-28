cmake_minimum_required (VERSION 3.8)

project (SharedMemory CXX)

my_find_src_files (src_files SharedMemory)
add_executable (SharedMemory ${src_files})
my_add_compile_options (SharedMemory)
my_add_pch (SharedMemory)


my_inc_import_RisLib(SharedMemory)
my_lib_import_RisLib(SharedMemory)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS SharedMemory DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  SharedMemory
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx SharedMemory rsync://debian@bby/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/SharedMemory)
endif()
