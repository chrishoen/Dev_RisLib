cmake_minimum_required (VERSION 3.8)

project (NamedSemaphore CXX)

my_find_src_files (src_files NamedSemaphore)
add_executable (NamedSemaphore ${src_files})
my_add_compile_options (NamedSemaphore)
my_add_pch (NamedSemaphore)


my_inc_import_RisLib(NamedSemaphore)
my_lib_import_RisLib(NamedSemaphore)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS NamedSemaphore DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  NamedSemaphore
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx NamedSemaphore rsync://debian@bby1/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/NamedSemaphore)
endif()
