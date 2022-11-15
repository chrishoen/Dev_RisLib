cmake_minimum_required(VERSION 3.10)

if (MSVC)
   install(TARGETS ${project} DESTINATION "C:/aaa_prime/RisLib")
elseif(MYMODE STREQUAL "prime-ubu")
   install(TARGETS ${project} DESTINATION "/opt/prime/bin")
elseif (MYMODE STREQUAL "prime-arm7")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND c:/windows/system32/openssh/scp ${CMAKE_CURRENT_BINARY_DIR}/${project} debian@bbx:/opt/prime/bin)")
elseif (MYMODE STREQUAL "prime-arm8")
   install(CODE "execute_process(COMMAND_ECHO STDOUT
      COMMAND scp ${CMAKE_CURRENT_BINARY_DIR}/${project} stenotgt:/opt/prime/bin)")
endif()

