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


return()

if (MSVC)
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS RisLib DESTINATION "C:/MyTools/MyLib/lib")
elseif(MyTargetType STREQUAL ubuntu)
   install(FILES ${inc_files} DESTINATION "/opt/prime/include/RisLib")
   install(TARGETS RisLib DESTINATION "/opt/prime/lib")
elseif(MyTargetType STREQUAL beagle)
   install(FILES ${inc_files} DESTINATION "C:/MyTools/MyLib/include/RisLib")
   install(TARGETS RisLib DESTINATION "C:/MyTools/MyLib/lib")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/libRisLib.so pi@rpx:/opt/prime/lib)")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/libRisLib.so debian@bbx:/opt/prime/lib)")
elseif(MyTargetType STREQUAL dart)
   install(FILES ${inc_files} DESTINATION "${SRC_LINUX_DIR}/gui/local/include/RisLib")
   install(TARGETS RisLib DESTINATION "${SRC_LINUX_DIR}/gui/local/lib")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/libRisLib.so root@dart:/home/root/stenograph/lib)")
endif()
