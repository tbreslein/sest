BIN_DIR := bin
HEADER := sest.h
EXAMPLE_DIR := examples
EXAMPLES_SRC := $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLES := $(subst $(EXAMPLE_DIR),$(BIN_DIR),$(patsubst %.c,%,$(EXAMPLES_SRC)))

# this stuff is only used for the examples
CC_DB := compile_commands.json
CC := gcc
CFLAGS := \
		  -std=c99 \
		  -Wall \
		  -Wextra \
		  -pedantic \
		  -Werror \
		  -Wfloat-equal \
		  -Wdouble-promotion \
		  -Winit-self \
		  -Wundef \
		  -Wshadow \
		  -Wformat=2 \
		  -Wunreachable-code \
		  -Wswitch-enum \
		  -Wswitch-default \
		  -O0

ifeq ($(CC),clang)
	CFLAGS += \
			  -Weverything \
			  -Wno-poison-system-directories \
			  -Wno-unused-function
endif
ifeq ($(CC),gcc)
	CFLAGS += \
			  -Wformat-truncation
endif

DEVFLAGS := $(CFLAGS) \
			-g2

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	DEVFLAGS += -fsanitize=address,undefined
endif

.PHONY: all dev clean run
all: $(EXAMPLES)

dev: $(EXAMPLES_SRC)
	@bear -- $(MAKE) CFLAGS="$(DEVFLAGS)"

run: $(EXAMPLES)
	@for e in $(EXAMPLES); do echo "Running example: $$e"; ./$$e || true; done

$(EXAMPLES): $(EXAMPLES_SRC) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(subst $(BIN_DIR),$(EXAMPLE_DIR),$@).c -I.

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -fr $(BIN_DIR)
	rm -fr $(CC_DB)
