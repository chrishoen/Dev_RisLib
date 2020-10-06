cmake_minimum_required (VERSION 3.8)

project (ThreadSynch CXX)

my_find_src_files (src_files ThreadSynch)
add_executable (ThreadSynch ${src_files})
my_add_compile_options (ThreadSynch)
my_add_pch (ThreadSynch)


my_inc_import_RisLib(ThreadSynch)

my_lib_import_RisLib(ThreadSynch)

if(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS ThreadSynch DESTINATION "/opt/prime/bin")
else()
   add_custom_command(
      TARGET  ThreadSynch
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx ThreadSynch rsync://debian@bbtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/ThreadSynch)
endif()
