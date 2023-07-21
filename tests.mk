CC      := gcc
CFLAGS  := -O2 -std=c17 -Wall -Wextra -pedantic -Isrc -g3 -fstack-protector -fsanitize=address,leak,undefined
LDFLAGS := 

SOURCE_DIR  := .
BUILD_DIR   := build
OBJECTS_DIR := $(BUILD_DIR)/tests

SOURCES := $(shell find $(SOURCE_DIR) -type f -name '*.c' | grep -v "main.c$$")
BINARY  := $(BUILD_DIR)/test

OBJECTS 	 := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECTS_DIR)/%.o, $(SOURCES))
DEPENDENCIES := $(patsubst %.o, %.d, $(OBJECTS))


.PHONY: all
all: $(BINARY)
	@./$(BINARY)

.PHONY: $(BINARY)
$(BINARY): $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJECTS_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPENDENCIES)