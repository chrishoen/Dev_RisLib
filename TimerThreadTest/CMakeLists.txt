cmake_minimum_required (VERSION 3.8)

project (TimerThreadTest CXX)

my_find_src_files (src_files TimerThreadTest)
add_executable (TimerThreadTest ${src_files})
my_add_compile_options (TimerThreadTest)
my_add_pch (TimerThreadTest)

my_inc_import_RisLib(TimerThreadTest)
my_inc_import_CommonLib(TimerThreadTest)

my_lib_import_CommonLib(TimerThreadTest)
my_lib_import_RisLib(TimerThreadTest)


if(CMAKE_SYSTEM_VERSION EQUAL 101)
   add_custom_command(  
      TARGET TimerThreadTest
      POST_BUILD
      COMMAND rsync -azv --chmod=ugo=rwx TimerThreadTest rsync://debian@devtgt/prime/bin
      WORKING_DIRECTORY C:/Users/$ENV{username}/CMakeBuilds/Dev_RisLib/TimerThreadTest)
endif()

