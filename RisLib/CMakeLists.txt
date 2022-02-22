cmake_minimum_required (VERSION 3.8)
set (project "RisLib")
project(${project} CXX)

my_find_src_files (src_files ${project})
my_find_inc_files (inc_files)
add_library (${project} STATIC ${src_files})
my_add_compile_options (${project})

my_add_pch (${project})

target_include_directories(${project} PUBLIC ${PROJECT_SOURCE_DIR})

if (MSVC)
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
elseif(MYMODE STREQUAL "cpdev1" OR MYMODE STREQUAL "wsl")
   install(FILES ${inc_files} DESTINATION "/opt/prime/include/RisLib")
   install(TARGETS ${project} DESTINATION "/opt/prime/lib")
elseif(MYMODE STREQUAL "beagle")
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
elseif(MYMODE STREQUAL "steno-arm")
   install(FILES ${inc_files} DESTINATION "include/RisLib")
   install(TARGETS ${project} DESTINATION "lib/arm")
elseif(MYMODE STREQUAL "steno-wsl")
   install(FILES ${inc_files} DESTINATION "include/RisLib")
   install(TARGETS RisLib DESTINATION "lib/x86")
endif()






return()
if (MSVC)
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
elseif(MYMODE STREQUAL "cpdev1" OR MYMODE STREQUAL "wsl")
   install(FILES ${inc_files} DESTINATION "/opt/prime/include/RisLib")
   install(TARGETS ${project} DESTINATION "/opt/prime/lib")
elseif(MYMODE STREQUAL "beagle")
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/libRisLib.so pi@rpx:/opt/prime/lib)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/libRisLib.so debian@bbx:/opt/prime/lib)")
elseif(MYMODE STREQUAL "steno-arm")
   install(FILES ${inc_files} DESTINATION "include/RisLib")
   install(TARGETS ${project} DESTINATION "lib/arm")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/lib${project}.so root@stenotgt:/home/root/stenograph/lib)")
elseif(MYMODE STREQUAL "steno-wsl")
   install(FILES ${inc_files} DESTINATION "include/RisLib")
   install(TARGETS RisLib DESTINATION "lib/x86")
endif()
