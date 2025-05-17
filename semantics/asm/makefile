# Makefile for x86-64 assembly projects (.asm files)

# Tools
ASM := as
LD := ld
ASMFLAGS := --64
LDFLAGS := 

# Files
SRCS := $(wildcard *.asm)
OBJS := $(SRCS:.asm=.o)
TARGET := main.exe

.PHONY: all clean

all: $(TARGET)

# Link all object files
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Assemble each .asm file to .o
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)