# Project include, source and library dirs for uarm-related files
set(UARM_INC ${PROJECT_SOURCE_DIR}/uarm)
set(UARM_SRC ${PROJECT_SOURCE_DIR}/uarm)

# Flags to be passed to the compiler
set(CFLAGS_UARM -mcpu=arm7tdmi -O0)
set(LINK_SCRIPT ${UARM_SRC}/elf32ltsarm.h.uarmcore.x)
# Flags to be passed to the linker. NOTE: the linker is not invoked directly,
# flags are passed to it via the -Wl compiler flag
set(LDFLAGS_UARM "-Wl,-nostdlib,-G,0,-T,${LINK_SCRIPT}")

# Set compiler and linker options
add_compile_options(${CFLAGS_UARM})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS_UARM}")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
# Specify #include<...> directories to look at
include_directories(${UARM_INC})
include_directories(${UARM_INC}/uarm)

add_library(libuarm ${UARM_SRC}/libuarm.s)
add_library(libdiv ${UARM_SRC}/libdiv.s)
add_library(crtso ${UARM_SRC}/crtso.s)

add_executable(kernel.uarm ${SRC}/hello.c)
target_link_libraries(kernel.uarm crtso libuarm termprint)