cmake_minimum_required (VERSION 3.8)

project (NamedSemaphore CXX)

my_find_src_files (src_files NamedSemaphore)
add_executable (NamedSemaphore ${src_files})
my_add_compile_options (NamedSemaphore)
my_add_pch (NamedSemaphore)


my_inc_import_RisLib(NamedSemaphore)
my_lib_import_RisLib(NamedSemaphore)

if(MSVC)
   install(TARGETS NamedSemaphore DESTINATION "C:/aaa_prime/RisLib")
elseif(NOT CMAKE_SYSTEM_VERSION EQUAL 101)
   install(TARGETS NamedSemaphore DESTINATION "/opt/prime/bin")
else()
   my_post_build_bin(NamedSemaphore)
endif()
