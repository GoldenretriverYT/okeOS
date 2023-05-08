# Nuke built-in rules and variables.
override MAKEFLAGS += -rR

# This is the name that our final kernel executable will have.
# Change as needed.
override KERNEL := kernel.elf

# Convenience macro to reliably declare user overridable variables.
define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef

define uniq =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

# It is highly recommended to use a custom built cross toolchain to build a kernel.
# We are only using "cc" as a placeholder here. It may work by using
# the host system's toolchain, but this is not guaranteed.
override CC := $$HOME/opt/cross/bin/x86_64-elf-gcc
# Same thing for "ld" (the linker).
$(eval $(call DEFAULT_VAR,LD,ld))

# User controllable C flags.
$(eval $(call DEFAULT_VAR,CFLAGS,-g -O2 -pipe -Wall -Wextra))

# User controllable C preprocessor flags. We set none by default.
$(eval $(call DEFAULT_VAR,CPPFLAGS,))

# User controllable nasm flags.
$(eval $(call DEFAULT_VAR,NASMFLAGS,-F dwarf -g))

# User controllable linker flags. We set none by default.
$(eval $(call DEFAULT_VAR,LDFLAGS,))

# Internal C flags that should not be changed by the user.
override CFLAGS += \
    -std=gnu11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-pie \
    -fno-pic \
    -fno-exceptions \
    -m64 \
    -march=x86-64 \
    -mabi=sysv \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel \
    -Wno-write-strings \
    -g

# Internal C preprocessor flags that should not be changed by the user.
override CPPFLAGS := \
    -I. \
    $(CPPFLAGS) \
    -MMD \
    -MP

# Internal linker flags that should not be changed by the user.
override LDFLAGS += \
    -nostdlib \
    --no-ld-generated-unwind-info \
    -static \
    -m elf_x86_64 \
    -z max-page-size=0x1000 \
    -T linker.ld \
    -Map kernel.map

# Check if the linker supports -no-pie and enable it if it does.
ifeq ($(shell $(LD) --help 2>&1 | grep 'no-pie' >/dev/null 2>&1; echo $$?),0)
    override LDFLAGS += -no-pie
endif

# Internal nasm flags that should not be changed by the user.
override NASMFLAGS += \
    -f elf64

# Use "find" to glob all *.c, *.S, and *.asm files in the tree and obtain the
# object and header dependency file names.
override CFILES := $(shell find -L . -type f -name '*.cpp')
override ASFILES := $(shell find -L . -type f -name '*.S')
override NASMFILES := $(shell find -L . -type f -name '*.asm')
override OFILES := $(shell find -L . -type f -name '*.o')
override INCLFILES := $(shell find -L . -type f -name '*.incbin')

override OBJ := $(CFILES:.cpp=.o) $(ASFILES:.S=.o) $(NASMFILES:.asm=.o) font.o
override HEADER_DEPS := $(CFILES:.cpp=.d) $(ASFILES:.S=.d)

$(info $(UNIQUEOBJS))

# Default target.
.PHONY: all
all: $(KERNEL)

limine.h:
	curl -Lo $@ https://github.com/limine-bootloader/limine/raw/trunk/limine.h

# Link rules for the final kernel executable.
$(KERNEL): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

# Include header dependencies.
-include $(HEADER_DEPS)

# Compilation rules for *.c files.
%.o: %.cpp limine.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.o: %.incbin
	objcopy -O elf64-x86-64 -B i386 -I binary $< $@

# Compilation rules for *.S files.
%.o: %.S limine.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
%.o: %.asm
	nasm $(NASMFLAGS) $< -o $@

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)

.PHONY: distclean
distclean: clean
	rm -f limine.h
