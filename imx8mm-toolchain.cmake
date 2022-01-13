SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_CXX_STANDARD_REQUIRED on)

#message(FATAL_ERROR "############# CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR} #############")

#set (YOCTO_BUILD_DIR "$ENV{HOME}/repos/nextgen/build_wayland/tmp")
#set (YOCTO_BUILD_DIR "$ENV{YOCTO_BUILD_DIR}")
set (YOCTO_BUILD_DIR "/home/main/work/nextgen/build_wayland/tmp")

if(NOT EXISTS ${YOCTO_BUILD_DIR})
   message(FATAL_ERROR "########## YOCTO_BUILD_DIR was not set correctly! YOCTO_BUILD_DIR:${YOCTO_BUILD_DIR} ###########")
endif()

set (SDKROOT "${YOCTO_BUILD_DIR}/work/aarch64-fslc-linux/nextgen-linux/0.1-r0")

set (CMAKE_SYSROOT "${SDKROOT}/recipe-sysroot" CACHE PATH "CMake sysroot22")
set (SYSROOT_COMPONENTS "${YOCTO_BUILD_DIR}/sysroots-components")
set (CROSS_COMPILER_PREFIX "${YOCTO_BUILD_DIR}/work/aarch64-fslc-linux/nextgen-linux/0.1-r0/recipe-sysroot-native/usr/bin/aarch64-fslc-linux/aarch64-fslc-linux-")

set (CMAKE_SYSTEM_NAME Linux)
set (CMAKE_SYSTEM_PROCESSOR aarch64)

set (CMAKE_C_COMPILER "${CROSS_COMPILER_PREFIX}gcc" CACHE FILEPATH "CMake C compiler22")
###set (CMAKE_C_FLAGS 
###"-O1 -DDEBUG -Wall -Wextra -fno-strict-aliasing -pipe -g -feliminate-unused-debug-types -frecord-gcc-switches -fno-omit-frame-pointer -mcpu=cortex-a53+crc+crypto -fstack-protector-strong  -D_FORTIFY_SOURCE=2 --sysroot=${CMAKE_SYSROOT}"
###)
# For optimized build, use -O3 -DNDEBUG instead
# Should use these flags also, but lot of warnings in the code now and they become errors with these flags:
#-Werror -Wformat -Wformat-security -Werror=format-security 

set (CMAKE_CXX_COMPILER "${CROSS_COMPILER_PREFIX}g++" CACHE FILEPATH "CMake CXX compiler22")
set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

SET(CMAKE_AR ${CROSS_COMPILER_PREFIX}ar)
SET(CMAKE_NM ${CROSS_COMPILER_PREFIX}nm)
SET(CMAKE_OBJCOPY ${CROSS_COMPILER_PREFIX}objcopy)
SET(CMAKE_OBJDUMP ${CROSS_COMPILER_PREFIX}objdump)
SET(CMAKE_RANLIB ${CROSS_COMPILER_PREFIX}ranlib)
SET(CMAKE_LINKER ${CROSS_COMPILER_PREFIX}ld)
SET(CMAKE_STRIP ${CROSS_COMPILER_PREFIX}strip)

set (CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

include_directories (SYSTEM 
    "${SDKROOT}/recipe-sysroot/usr/include/"
    )
link_directories(
    "${SDKROOT}/recipe-sysroot/usr/lib/"
    "${SDKROOT}/recipe-sysroot/lib/"
    "${SYSROOT_COMPONENTS}/aarch64/libgcc/usr/lib/aarch64-fslc-linux/9.3.0/"
    "${SYSROOT_COMPONENTS}/aarch64/libgcc/lib/"
    "${SYSROOT_COMPONENTS}/aarch64/glibc/lib/"
    )


