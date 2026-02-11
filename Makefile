CC = gcc
CFLAGS = -Wall -g -O0
DEBUG_FLAGS = -DDEBUG
BUILD_DIR = build

all: $(BUILD_DIR)/example

debug:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) $(DEBUG_FLAGS)" all

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/libmy_alloc.a: $(BUILD_DIR)/my_alloc.o
	ar rcs $(BUILD_DIR)/libmy_alloc.a $(BUILD_DIR)/my_alloc.o

$(BUILD_DIR)/my_alloc.o: my_alloc.c my_alloc.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c my_alloc.c -o $(BUILD_DIR)/my_alloc.o

$(BUILD_DIR)/example: example.c $(BUILD_DIR)/libmy_alloc.a
	$(CC) $(CFLAGS) example.c -o $(BUILD_DIR)/example $(BUILD_DIR)/libmy_alloc.a

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all debug clean
