cmake_minimum_required (VERSION 3.8)

project (RisLib CXX)

my_find_src_files (src_files RisLib)
my_find_inc_files (inc_files)
add_library (RisLib STATIC ${src_files})
my_add_compile_options (RisLib)
my_add_pch (RisLib)

target_include_directories(RisLib PUBLIC ${PROJECT_SOURCE_DIR})

if (MSVC)
   install(FILES ${inc_files} DESTINATION "C:\\MyTools\\MyLib\\include\\RisLib")
   install(TARGETS RisLib DESTINATION "C:\\MyTools\\MyLib\\lib")
else()
   install(FILES ${inc_files} DESTINATION "/usr/local/include/RisLib")
   install(TARGETS RisLib DESTINATION "/usr/local/lib")
endif()
