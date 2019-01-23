cmake_minimum_required (VERSION 3.8)

project (TimerTest CXX)

my_find_src_files (src_files TimerTest)
add_executable (TimerTest ${src_files})
my_add_compile_options (TimerTest)
my_add_pch (TimerTest)

target_include_directories(TimerTest PRIVATE RisLib)
target_include_directories(TimerTest PRIVATE CommonLib)

target_link_libraries(TimerTest RisLib)
target_link_libraries(TimerTest CommonLib)
