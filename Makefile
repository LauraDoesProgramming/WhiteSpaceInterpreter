SOURCE_FILES := $(shell find ./src -type f -name "*.cpp")
DEST_DIR := ./dest
OUTPUT_NAME := whitespace
COMPILER := g++
W_RELAXED := -Wall -Wextra -pedantic
W_FLAGS := $(W_RELAXED) -Werror
TESTS = ./tests
MAKETEST_WS = maketest.ws


dry:
	@echo $(COMPILER) -g $(SOURCE_FILES) $(W_FLAGS) -o $(DEST_DIR)/$(OUTPUT_NAME)

release:
	$(COMPILER) $(SOURCE_FILES) $(W_FLAGS) -O3 -o $(DEST_DIR)/$(OUTPUT_NAME)

build:
	$(COMPILER) -g $(SOURCE_FILES) $(W_FLAGS) -o $(DEST_DIR)/$(OUTPUT_NAME)

relaxed:
	$(COMPILER) -g $(SOURCE_FILES) $(W_RELAXED) -o $(DEST_DIR)/$(OUTPUT_NAME)

run: build
	@$(DEST_DIR)/$(OUTPUT_NAME) $(TESTS)/$(MAKETEST_WS)

clean:
	@rm $(DEST_DIR)/*