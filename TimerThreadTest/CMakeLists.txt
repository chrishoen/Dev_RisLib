cmake_minimum_required (VERSION 3.8)

project (TimerThreadTest CXX)

my_find_src_files (src_files TimerThreadTest)
add_executable (TimerThreadTest ${src_files})
my_add_compile_options (TimerThreadTest)
my_add_pch (TimerThreadTest)

target_include_directories(TimerThreadTest PRIVATE RisLib)
target_include_directories(TimerThreadTest PRIVATE CommonLib)

target_link_libraries(TimerThreadTest RisLib)
target_link_libraries(TimerThreadTest CommonLib)
