cmake_minimum_required (VERSION 3.8)
set (project "RisLib")
project(${project} CXX)

my_find_src_files (src_files ${project})
my_find_inc_files (inc_files)
add_library (${project} STATIC ${src_files})
my_add_compile_options (${project})

my_add_pch (${project})

target_include_directories(${project} PUBLIC ${PROJECT_SOURCE_DIR})

if (MYSTANDALONE)
   return()
endif()

if (MSVC)
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
elseif(MYMODE STREQUAL "prime-x64")
   install(FILES ${inc_files} DESTINATION "/opt/prime/include/RisLib")
   install(TARGETS ${project} DESTINATION "/opt/prime/lib/x64")
elseif(MYMODE STREQUAL "prime-arm7")
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
elseif(MYMODE STREQUAL "prime-arm8")
   install(FILES ${inc_files} DESTINATION "/opt/prime/include/RisLib")
   install(TARGETS ${project} DESTINATION "/opt/prime/lib/arm8")
endif()




return()
if (MSVC)
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
#elseif(MYMODE STREQUAL "cpdev1" OR MYMODE STREQUAL "wsl" OR MYMODE STREQUAL "wsl-cpdev1")
elseif(MYMODE STREQUAL "cpdev1" OR MYMODE STREQUAL "wsl")
   install(FILES ${inc_files} DESTINATION "/opt/prime/include/RisLib")
   install(TARGETS ${project} DESTINATION "/opt/prime/lib/x64")
elseif(MYMODE STREQUAL "beagle")
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS ${project} DESTINATION "C:/MyTools/MyLib/lib")
elseif(MYMODE STREQUAL "steno-arm")
   install(FILES ${inc_files} DESTINATION "target/rislib/include")
   install(TARGETS ${project} DESTINATION "target/rislib/lib/arm")
elseif(MYMODE STREQUAL "steno-wsl")
   install(FILES ${inc_files} DESTINATION "target/rislib/include")
   install(TARGETS RisLib DESTINATION "target/rislib/lib/x86")
endif()

