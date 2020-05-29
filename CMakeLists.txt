cmake_minimum_required(VERSION 3.3)
project(BiKayaOS LANGUAGES C ASM)  

# set variables for all directories
set(SRC ${PROJECT_SOURCE_DIR})
set(EXC ${PROJECT_SOURCE_DIR}/exception_hndlr/)
set(GNR ${PROJECT_SOURCE_DIR}/generics/)
set(INC ${PROJECT_SOURCE_DIR}/include/)
set(PRC ${PROJECT_SOURCE_DIR}/process/)
set(DEV ${PROJECT_SOURCE_DIR}/devices/)


# Set C standard and compilation flags
set(CMAKE_C_STANDARD 99)
set(CFLAGS_LANG -ffreestanding -Wall)

# PROJECT_SOURCE_DIR e PROJECT_BINARY_DIR are CMake variables
include_directories(${SRC})
include_directories(${DEV})
include_directories(${EXC})
include_directories(${GNR})
include_directories(${INC})
include_directories(${PRC})
add_compile_options(${CFLAGS_LANG})

# Conditional inclusion of targets upon the detected architecture
if (CMAKE_SYSTEM_PROCESSOR STREQUAL mips)
	add_definitions(-DTARGET_UMPS)
	include(uMPS.cmake)

elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
	add_definitions(-DTARGET_UARM)
	include(uARM.cmake)


else()
	message(FATAL_ERROR "You need to specify a toolchain file with either -D
	CMAKE_TOOLCHAIN_FILE=toolchains/uMPS.cmake or -D
	CMAKE_TOOLCHAIN_FILE=toolchains/uARM.cmake")

endif()
