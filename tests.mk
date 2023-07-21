CC      := g++
CFLAGS  := -O2 -std=c++17 -Wall -Wextra -pedantic -Isrc -g3 -fstack-protector -fsanitize=address,leak,undefined
LDFLAGS := 

ifdef TEST
	CFLAGS += -DTEST=$(TEST)
endif

SOURCE_DIR  := .
BUILD_DIR   := build
OBJECTS_DIR := $(BUILD_DIR)/tests

SOURCES := $(shell find $(SOURCE_DIR) -type f -name '*.cc' | grep -v "main.cc$$")
BINARY  := $(BUILD_DIR)/test

OBJECTS 	 := $(patsubst $(SOURCE_DIR)/%.cc, $(OBJECTS_DIR)/%.o, $(SOURCES))
DEPENDENCIES := $(patsubst %.o, %.d, $(OBJECTS))


.PHONY: all
all: $(BINARY)
	@cppcheck --enable=warning,performance,missingInclude **/*.cc **/*.hh
	@./$(BINARY)

.PHONY: $(BINARY)
$(BINARY): $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJECTS_DIR)/%.o: $(SOURCE_DIR)/%.cc
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPENDENCIES)
