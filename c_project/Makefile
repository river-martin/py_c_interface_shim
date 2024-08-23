##
# Makefile for a C library that will be used by a Python C extension module
# @author River Martin (adapted from file by Brendan Watling)

# flags that enable Address Sanitizer
# Note: run `export LD_PRELOAD=$(clang -print-file-name=libclang_rt.asan-x86_64.so)` if you want to use Address Sanitizer
ASAN_FLAGS ?= # -fsanitize=address -shared-libasan

# compiler flags
DEBUG     := -ggdb -gdwarf-4 $(ASAN_FLAGS)
OPTIMISE  := -O0
WARNING   := -Wall -Wextra -Wno-variadic-macros \
             -Wno-overlength-strings -pedantic
EXTRA     := -std=c11 -fPIC
CFLAGS    := $(DEBUG) $(OPTIMISE) $(WARNING) $(EXTRA)
DFLAGS    ?= # -DDEBUG

# linker flags
LDFLAGS	 	:= -shared $(ASAN_FLAGS)

# commands
CC        := clang
RM        := rm -f
COMPILE   := $(CC) $(CFLAGS) $(DFLAGS)

# directories
SRCDIR    := src
BINDIR    := bin

# files
C_PROJECT_EXE := c_interface_consumer
C_PROJECT_SO := libcproject.so
SO := $(C_PROJECT_SO)

C_PROJECT_SRC := $(SRCDIR)/$(C_PROJECT_EXE).c
EXE_SRC   := $(C_PROJECT_SRC)

SRC       := $(filter-out $(EXE_SRC), $(wildcard $(SRCDIR)/*.c))
OBJ       := $(SRC:.c=.o)

### RULES ######################################################################

# shared objects

$(C_PROJECT_SO): $(C_PROJECT_SRC) $(OBJ) | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^ $(LDFLAGS)

# units

%.o: %.c
	$(COMPILE) -c -o $@ $<

# BINDIR

$(BINDIR):
	mkdir -p $(BINDIR)

### PHONY TARGETS ##############################################################

all: libcproject

libcproject: $(C_PROJECT_SO)

clean: cleanobj cleanbin

cleanobj:
	$(RM) $(OBJ)

cleanbin:
	$(RM) $(addprefix $(BINDIR)/, $(C_PROJECT_SO))

.PHONY: all libcproject clean cleanobj cleanbin

# end
