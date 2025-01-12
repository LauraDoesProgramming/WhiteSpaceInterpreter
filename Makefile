SOURCE_FILES := $(shell find ./src -type f -name "*.cpp")
DEST_DIR := ./dest
OUTPUT_NAME := whitespace
COMPILER := g++
W_RELAXED := -Wall -Wextra -pedantic
W_FLAGS := $(W_RELAXED) -Werror
TESTS = ./tests
MAKETEST_WS = maketest.ws
MKDIR = mkdir -p dest

build:
	$(MKDIR)
	$(COMPILER) -g $(SOURCE_FILES) $(W_FLAGS) -o $(DEST_DIR)/$(OUTPUT_NAME)

dry:
	@echo $(COMPILER) -g $(SOURCE_FILES) $(W_FLAGS) -o $(DEST_DIR)/$(OUTPUT_NAME)

release:
	$(MKDIR)
	$(COMPILER) $(SOURCE_FILES) $(W_FLAGS) -O3 -o $(DEST_DIR)/$(OUTPUT_NAME)

relaxed:
	$(MKDIR)
	$(COMPILER) -g $(SOURCE_FILES) $(W_RELAXED) -o $(DEST_DIR)/$(OUTPUT_NAME)

run: build
	@$(DEST_DIR)/$(OUTPUT_NAME) $(TESTS)/$(MAKETEST_WS)

clean:
	$(MKDIR)
	@rm $(DEST_DIR)/*