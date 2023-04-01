cmake_minimum_required (VERSION 3.8)

set (project "MyProgram1")
project(${project} CXX)
my_find_src_files (src_files ${project})
add_executable (${project} ${src_files})
my_add_pch (${project})


include("${MY_MACRO_DIR}/MyInstall.cmake")


return()
if (MSVC)
   install(TARGETS MyProgram1 DESTINATION "C:/aaa_prime/RisLib")
elseif (MYMODE STREQUAL "cpdev1" OR MYMODE STREQUAL "wsl") 
   install(TARGETS MyProgram1 DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL "beagle")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/MyProgram1 pi@rpx:/opt/prime/bin)")
elseif (MYMODE STREQUAL "steno-arm")

   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/MyProgram1 stenotgt:/opt/prime/bin)")
endif()

