NAME := image-transformer

BUILDDIR = build
OBJDIR = obj
SOLUTION_DIR = solution

CFLAGS= -std=c17 -pedantic -Wall -O3 -g -c
ASMFLAGS = -g -f elf64

RM = rm -rf

CC = gcc
LINKER = $(CC)
ASM = nasm

SOLUTION = solution

SRC_DIR = $(SOLUTION_DIR)/src

MKDIR = mkdir -p

all: $(OBJDIR) $(BUILDDIR)  $(OBJDIR)/bmp_reader.o  $(OBJDIR)/bmp_writer.o  $(OBJDIR)/padding.o $(OBJDIR)/file_works.o $(OBJDIR)/image_format.o $(OBJDIR)/rotate.o $(OBJDIR)/saturation_arithmetic.o $(OBJDIR)/sepia.o $(OBJDIR)/sepia_asm.o $(OBJDIR)/util.o $(OBJDIR)/main.o
	$(LINKER) -g -o $(BUILDDIR)/main $(OBJDIR)/*.o

$(OBJDIR)/bmp_reader.o: $(SRC_DIR)/bmp/bmp_reader.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/bmp_writer.o: $(SRC_DIR)/bmp/bmp_writer.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/padding.o: $(SRC_DIR)/bmp/padding.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/file_works.o: $(SRC_DIR)/file_works/file_works.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/image_format.o: $(SRC_DIR)/image_format/image_format.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/rotate.o: $(SRC_DIR)/rotate/rotate.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/saturation_arithmetic.o: $(SRC_DIR)/sepia/saturation_arithmetic.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/sepia.o: $(SRC_DIR)/sepia/sepia.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/sepia_asm.o: $(SRC_DIR)/sepia/sepia_asm.ASM
	$(ASM) $(ASMFLAGS) $< -o $@

$(OBJDIR)/util.o: $(SRC_DIR)/util/util.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/main.o: $(SRC_DIR)/main.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR):
	$(MKDIR) $@

$(BUILDDIR):
	$(MKDIR) $@

clean:
	$(RM) $(OBJDIR) $(BUILDDIR)

run: $(BUILDDIR)/main
	$(BUILDDIR)/main input.bmp out.bmp