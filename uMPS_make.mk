# Cross toolchain variables
# If these are not in your path, you can make them absolute.
XT_PRG_PREFIX = mipsel-linux-gnu-
CC = $(XT_PRG_PREFIX)gcc
LD = $(XT_PRG_PREFIX)ld

# uMPS2-related paths
UMPS2_DIR = ./include/uMPS

GENERICS_DIR = ./generics
EXC_HNDL_DIR = ./exception_handler
DEVICE_DIR = ./device
PROCESS_DIR = ./process
INCLUDE_DIR = ./include

# Compiler options
CFLAGS_LANG = -ffreestanding -DTARGET_UMPS=1
CFLAGS_MIPS = -mips1 -mabi=32 -mno-gpopt -G 0 -mno-abicalls -fno-pic -mfp32
LINKER_INCLUDE_OPT = -I$(DEVICE_DIR) -I$(PROCESS_DIR) -I$(GENERICS_DIR) -I$(EXC_HNDL_DIR) -I$(UARM_DIR) -I$(UARM_DIR)/uarm -I$(INCLUDE_DIR)
CFLAGS = $(CFLAGS_LANG) $(CFLAGS_MIPS) -I$(LINKER_INCLUDE_OPT) -DUMPS=1 -Wall -O0

# Linker options
LDFLAGS = -G 0 -nostdlib -T $(UMPS2_DIR)/umpscore.ldscript

# Add the location of crt*.S to the search path
VPATH = $(UMPS2_DIR)

.PHONY : all clean

all : kernel.core.umps

kernel.core.umps : kernel
	umps2-elf2umps -k $<
	
kernel : phase1,5_test.o generics/utils.o exception_handler/syscall_breakpoint.o process/scheduler.o process/pcb.o process/asl.o crtso.o libumps.o
	$(LD) -o $@ $^ $(LDFLAGS)

clean :
	-rm -rf *.o kernel kernel.*.umps
	-rm $(DEVICE_DIR)/*.o $(PROCESS_DIR)/*.o $(GENERICS_DIR)/*.o $(EXC_HNDL_DIR)/*.o

# Pattern rule for assembly modules
%.o : %.S
	$(CC) $(CFLAGS) -c -o $@ $<
