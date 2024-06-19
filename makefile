BUILD_DIR := build
HEADER := sest.h
EXAMPLE_DIR := examples
EXAMPLES_SRC := $(wildcard $(EXAMPLE_DIR)/*.c)
EXAMPLES := $(subst $(EXAMPLE_DIR),$(BUILD_DIR),$(patsubst %.c,%,$(EXAMPLES_SRC)))

# this stuff is only used for the examples
CC_DB := compile_commands.json
CC := gcc
CFLAGS := \
		  -std=c23 \
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
		  -Wformat-truncation \
		  -Wunreachable-code \
		  -Wswitch-enum \
		  -Wswitch-default \
		  -flto \
		  -Os

ifeq ($(CC),clang)
	CFLAGS += -Weverything
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

$(EXAMPLES): $(EXAMPLES_SRC) $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(subst $(BUILD_DIR),$(EXAMPLE_DIR),$@).c -I.

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -fr $(BUILD_DIR)
	rm -fr $(CC_DB)
