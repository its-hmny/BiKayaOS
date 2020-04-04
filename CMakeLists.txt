cmake_minimum_required(VERSION 3.3)
project(BiKayaOS)  

# set variables for all directories
set(SRC ${PROJECT_SOURCE_DIR})
set(DVC ${PROJECT_SOURCE_DIR}/device/)
set(EXC ${PROJECT_SOURCE_DIR}/exception_handler/)
set(GNR ${PROJECT_SOURCE_DIR}/generics/)
set(INC ${PROJECT_SOURCE_DIR}/include/)
set(PRC ${PROJECT_SOURCE_DIR}/process/)


# Set C standard and compilation flags
set(CMAKE_C_STANDARD 90)
set(CFLAGS_LANG -ffreestanding -ansi -Wall)

# PROJECT_SOURCE_DIR e PROJECT_BINARY_DIR are CMake variables
include_directories(${SRC})
include_directories(${DVC})
include_directories(${EXC})
include_directories(${GNR})
include_directories(${INC})
include_directories(${PRC})
add_compile_options(${CFLAGS_LANG})

# Conditional inclusion of targets upon the detected architecture
if (CMAKE_SYSTEM_PROCESSOR STREQUAL mips)
	add_definitions(-DTARGET_UMPS)
	include(umps.cmake)

elseif (CMAKE_SYSTEM_PROCESSOR STREQUAL arm)
	add_definitions(-DTARGET_UARM)
	include(uarm.cmake)

else()
	message(FATAL_ERROR "You need to specify a toolchain file with either -D
	CMAKE_TOOLCHAIN_FILE=toolchains/umps.cmake or -D
	CMAKE_TOOLCHAIN_FILE=toolchains/uarm.cmake")

endif()

# Note: defining libraries before the include() block above may lead to bugs,
# for special compiler/linker flags would not be assigned to them
add_library(termprint ${SRC}/termprint.c)
