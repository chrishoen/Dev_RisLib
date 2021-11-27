cmake_minimum_required (VERSION 3.8)

project (RisLib CXX)

my_find_src_files (src_files RisLib)
my_find_inc_files (inc_files)
if (MSVC)
   add_library (RisLib STATIC ${src_files})
else()
   add_library (RisLib SHARED ${src_files})
endif()
my_add_compile_options (RisLib)
my_add_pch (RisLib)

target_include_directories(RisLib PUBLIC ${PROJECT_SOURCE_DIR})

if (NOT MSVC)
   #target_link_options(RisLib PUBLIC "-lpthread")
   #target_link_options(RisLib PUBLIC "-lrt")
endif()

if (MSVC)
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS RisLib DESTINATION "C:/MyTools/MyLib/lib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(FILES ${inc_files} DESTINATION "/opt/prime/include/RisLib")
   install(TARGETS RisLib DESTINATION "/opt/prime/lib")
else()
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS RisLib DESTINATION "C:/MyTools/MyLib/lib")
   my_post_build_lib(RisLib)
endif()

