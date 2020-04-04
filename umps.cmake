# Project include, source and library dirs for umips-related files
set(UMPS_INC ${PROJECT_SOURCE_DIR}/umps)
set(UMPS_SRC ${PROJECT_SOURCE_DIR}/umps)

# Flags to be passed to gcc
set(
	CFLAGS_UMPS
	-ffreestanding -ansi -mips1 -mabi=32 -mno-gpopt -G 0 # -mabicalls -fPIC
	-mfp32 -Wall -O0
)
set(LINK_SCRIPT ${UMPS_SRC}/umpscore.ldscript)
# Flags to be passed to ld
set(LDFLAGS_UMPS "-nostdlib -Wl,-G,0,-nostdlib,-T,${LINK_SCRIPT}")

add_compile_options(${CFLAGS_UMPS})
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS_UMPS}")
# set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
# Specify #include<...> directories to look at
include_directories(${UMPS_INC})
include_directories(${UMPS_INC}/umps)

add_library(crtso ${UMPS_SRC}/crtso.S)
add_library(libumps ${UMPS_SRC}/libumps.S)

add_executable(kernel ${SRC}/hello.c)
target_link_libraries(kernel crtso libumps termprint)

# Run the `umps2-elf2umps -k kernel' command after building `kernel'
add_custom_target(
	kernel.core.umps ALL
	COMMAND umps2-elf2umps -k ${PROJECT_BINARY_DIR}/kernel
	BYPRODUCTS kernel.core.umps kernel.stab.umps
	DEPENDS kernel
)